#include "raylib.h"
#include "raymath.h"

// --- SIMPLE LIGHTING SHADER ---
const char* vsCode = R"(
    #version 330
    in vec3 vertexPosition;
    in vec2 vertexTexCoord;
    in vec3 vertexNormal;
    uniform mat4 mvp;
    uniform mat4 matModel;
    uniform mat4 matNormal;
    out vec3 fragNormal;
    void main() {
        fragNormal = normalize(vec3(matNormal * vec4(vertexNormal, 1.0)));
        gl_Position = mvp * vec4(vertexPosition, 1.0);
    }
)";

const char* fsCode = R"(
    #version 330
    in vec3 fragNormal;
    out vec4 finalColor;
    void main() {
        vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
        float light = max(dot(fragNormal, lightDir), 0.0) + 0.3;
        finalColor = vec4(light, light, light, 1.0);
    }
)";

// Define our modes
enum ControlMode {
    MODE_CAMERA,
    MODE_OBJ_TRANSLATE,
    MODE_OBJ_ROTATE,
    MODE_OBJ_SCALE
};

int main(void)
{
    // 1. Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "3D Model Manipulator");
    SetExitKey(KEY_NULL); // ESC is Pause, not Exit

    // Camera Setup
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Load Model & Shader
    Model model = LoadModel("christ.glb");
    Shader shader = LoadShaderFromMemory(vsCode, fsCode);
    model.materials[0].shader = shader;

    // Object Transform Variables
    Vector3 objPos = { 0.0f, 0.0f, 0.0f };
    Vector3 objRot = { 0.0f, 0.0f, 0.0f }; // Pitch, Yaw, Roll
    Vector3 objScale = { 1.0f, 1.0f, 1.0f };

    // State Variables
    ControlMode currentMode = MODE_CAMERA;
    bool isPaused = false;
    DisableCursor(); 

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // --- GLOBAL INPUTS ---
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        if (IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
            if (isPaused) EnableCursor(); else DisableCursor();
        }

        if (!isPaused)
        {
            // Toggle Modes using TAB
            if (IsKeyPressed(KEY_TAB)) {
                if (currentMode == MODE_CAMERA) currentMode = MODE_OBJ_TRANSLATE;
                else currentMode = MODE_CAMERA;
            }

            // Switch Object Manipulation Type (Only if NOT in Camera Mode)
            if (currentMode != MODE_CAMERA) {
                if (IsKeyPressed(KEY_ONE)) currentMode = MODE_OBJ_TRANSLATE;
                if (IsKeyPressed(KEY_TWO)) currentMode = MODE_OBJ_ROTATE;
                if (IsKeyPressed(KEY_THREE)) currentMode = MODE_OBJ_SCALE;
                if (IsKeyPressed(KEY_R)) { 
                    objPos = (Vector3){0,0,0}; 
                    objRot = (Vector3){0,0,0}; 
                    objScale = (Vector3){1,1,1}; 
                }
            }

            // --- CONTROLS LOGIC ---
            float speed = 0.1f; 
            
            // --- 1. CAMERA MODE ---
            if (currentMode == MODE_CAMERA) {
                UpdateCamera(&camera, CAMERA_FIRST_PERSON);
                
                // Manual Up (Space) / Down (Shift) for Camera
                if (IsKeyDown(KEY_SPACE)) {
                    camera.position.y += speed;
                    camera.target.y += speed;
                }
                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    camera.position.y -= speed;
                    camera.target.y -= speed;
                }
            }
            // --- 2. OBJECT MODE ---
            else {
                Vector3 input = { 0, 0, 0 };
                
                // Standard WASD
                if (IsKeyDown(KEY_W)) input.z -= 1; // Forward
                if (IsKeyDown(KEY_S)) input.z += 1; // Back
                if (IsKeyDown(KEY_A)) input.x -= 1; // Left
                if (IsKeyDown(KEY_D)) input.x += 1; // Right
                
                // REPLACED Q/E with SPACE/SHIFT
                if (IsKeyDown(KEY_LEFT_SHIFT)) input.y -= 1; // Down
                if (IsKeyDown(KEY_SPACE))      input.y += 1; // Up

                // Apply logic based on sub-mode
                if (currentMode == MODE_OBJ_TRANSLATE) {
                    objPos.x += input.x * speed;
                    objPos.y += input.y * speed; // Space moves up, Shift moves down
                    objPos.z += input.z * speed;
                }
                else if (currentMode == MODE_OBJ_ROTATE) {
                    // W/S = Pitch, A/D = Yaw, Space/Shift = Roll
                    objRot.x += input.z * 2.0f; 
                    objRot.y += input.x * 2.0f; 
                    objRot.z += input.y * 2.0f; // Space rolls one way, Shift rolls other
                }
                else if (currentMode == MODE_OBJ_SCALE) {
                    // W grows, S shrinks
                    float scaleDelta = -input.z * 0.05f; 
                    objScale.x += scaleDelta;
                    objScale.y += scaleDelta;
                    objScale.z += scaleDelta;
                    if (objScale.x < 0.1f) objScale = (Vector3){0.1f, 0.1f, 0.1f};
                }
            }
        }

        // --- APPLY TRANSFORMATIONS ---
        // Create matrices for Scale, Rotation, and Translation
        Matrix matScale = MatrixScale(objScale.x, objScale.y, objScale.z);
        Matrix matRot = MatrixRotateXYZ((Vector3){ DEG2RAD*objRot.x, DEG2RAD*objRot.y, DEG2RAD*objRot.z });
        Matrix matTrans = MatrixTranslate(objPos.x, objPos.y, objPos.z);
        
        // Combine them: Scale -> Rotate -> Translate
        model.transform = MatrixMultiply(MatrixMultiply(matScale, matRot), matTrans);

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawModel(model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
                DrawGrid(20, 1.0f);
            EndMode3D();

            // UI
            if (isPaused) {
                DrawText("PAUSED", 10, 10, 20, BLACK);
            } else {
                DrawText("TAB: Toggle Camera/Object Mode", 10, 10, 20, DARKGRAY);
                
                if (currentMode == MODE_CAMERA) {
                    DrawText("MODE: CAMERA", 10, 40, 30, BLUE);
                    DrawText("WASD + Space/Shift", 10, 70, 20, BLUE);
                } else {
                    const char* modeStr = "UNKNOWN";
                    if (currentMode == MODE_OBJ_TRANSLATE) modeStr = "TRANSLATE (Move)";
                    if (currentMode == MODE_OBJ_ROTATE)    modeStr = "ROTATE (Spin)";
                    if (currentMode == MODE_OBJ_SCALE)     modeStr = "SCALE (Size)";
                    
                    DrawText(TextFormat("MODE: %s", modeStr), 10, 40, 30, MAROON);
                    DrawText("Controls: WASD + Space/Shift", 10, 70, 20, DARKGRAY);
                    DrawText("Keys: 1=Move, 2=Rotate, 3=Scale, R=Reset", 10, 95, 20, DARKGRAY);
                }
                
                // Debug Stats at bottom
                DrawText(TextFormat("Pos: %.1f, %.1f, %.1f", objPos.x, objPos.y, objPos.z), 10, GetScreenHeight()-90, 20, BLACK);
                DrawText(TextFormat("Rot: %.1f, %.1f, %.1f", objRot.x, objRot.y, objRot.z), 10, GetScreenHeight()-60, 20, BLACK);
                DrawText(TextFormat("Scl: %.2f", objScale.x), 10, GetScreenHeight()-30, 20, BLACK);
            }

        EndDrawing();
    }

    UnloadShader(shader);
    UnloadModel(model);
    CloseWindow();

    return 0;
}