#ifndef FPCAMERA_H
#define FPCAMERA_H

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class FPCamera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 forward;
    glm::vec3 up;
    float camheight;

    FPCamera();
    ~FPCamera();

    void Init(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0,1,0));
    void Reset();
    void Update(double dt);
    glm::vec3 GetView(void);
    glm::vec3 GetFront(void);
    float GetStamina(void);
    void RotateCamera(float dt);
    void RotateAboutCamera(glm::vec3 rotpt, float angle);

    //bools for camera mechanics

    bool allowMovement = true; // 
    bool allowJump = true; // 
    bool allowSprint = true; // 
    bool allowCrouch = true; // 
    bool allowProne = true; // 
    bool allowLocomotiveTilt = true;
    bool allowLocomotiveBop = true;

private:

    bool isDirty;   // indicate if there is a need to recalculate the camera attributes
    void Refresh();
    
    static float currentPitch;

    float currentangle;
    float amplitude;
    float frequency;
    float time;
    float jumpduration;
    float crouchduration;
    float proneduration;
    bool jumpflag;
    bool crouchflag;
    bool jumpphase1;
    bool crouchphase1;
    bool proneflag;
    bool pronephase1;

    float swaytimer;
    float swayduration;
    bool swayphase1;
    float sway;

    float boptimer;
    float bop;
    bool bopflag;

    float sprintstamina;
    bool staminaflag;
    bool runflag;
    bool heightresetflag = false;

    signed short int multDebugX, multDebugZ;

};

#endif

