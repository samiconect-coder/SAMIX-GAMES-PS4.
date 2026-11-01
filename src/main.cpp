
#include <orbis/Pad.h>
#include <orbis/UserService.h>
#include "graphics.h"
#include <stdint.h>

#define WIDTH 1920
#define HEIGHT 1080

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    Scene2D scene(WIDTH, HEIGHT, 4);

    if (!scene.Init(0xC000000, 2))
        return 1;

    scePadInit();

    OrbisUserServiceInitializeParams params;
    params.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&params);

    int userID = 0;
    sceUserServiceGetInitialUser(&userID);

    int pad = scePadOpen(userID, 0, 0, NULL);

    if (pad < 0)
        return 2;

    int selected = 0;
    int previousButtons = 0;
    int frame = 0;

    Color background = {5, 10, 28};
    Color blue = {0, 100, 220};
    Color bright = {0, 190, 255};
    Color white = {220, 240, 255};
    Color dark = {10, 25, 55};

    for (;;)
    {
        OrbisPadData data;
        scePadReadState(pad, &data);

        int buttons = data.buttons;
        int pressed = buttons & ~previousButtons;
        previousButtons = buttons;

        if (pressed & ORBIS_PAD_BUTTON_RIGHT)
            selected = (selected + 1) % 22;

        if (pressed & ORBIS_PAD_BUTTON_LEFT)
            selected = (selected + 21) % 22;

        if (pressed & ORBIS_PAD_BUTTON_DOWN)
            selected = (selected + 4) % 22;

        if (pressed & ORBIS_PAD_BUTTON_UP)
            selected = (selected + 18) % 22;

        scene.FrameBufferFill(background);

        // Header
        scene.DrawRectangle(60, 50, 1800, 8, bright);
        scene.DrawRectangle(60, 70, 1800, 100, dark);
        scene.DrawRectangle(60, 180, 1800, 3, blue);

        // Game cards
        for (int i = 0; i < 22; i++)
        {
            int col = i % 4;
            int row = i / 4;

            int x = 90 + col * 445;
            int y = 240 + row * 145;

            Color card = dark;

            if (i == selected)
                card = bright;
            else
                card = blue;

            scene.DrawRectangle(x, y, 390, 105, card);
            scene.DrawRectangle(x + 5, y + 5, 380, 95, dark);

            // Number marker
            scene.DrawRectangle(x + 18, y + 25, 45, 55, card);
        }

        // Bottom controller guide
        scene.DrawRectangle(60, 1000, 1800, 4, bright);
        scene.DrawRectangle(80, 1020, 500, 25, blue);
        scene.DrawRectangle(650, 1020, 500, 25, blue);
        scene.DrawRectangle(1220, 1020, 500, 25, blue);

        scene.SubmitFlip(frame);
        scene.FrameWait(frame);
        scene.FrameBufferSwap();
        frame++;
    }

    return 0;
}
