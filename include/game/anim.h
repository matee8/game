#ifndef ANIM_H
#define ANIM_H

#include <raylib.h>

struct anim{
    int frame_count;
    Image gif_anim;
    Texture2D tex_anim;
    unsigned int next_frame_offset;
    int current_frame;
    int frame_delay;
    int frame_counter;
};

void init_anim(struct anim* animation, const char* path, int frame_delay);
void update_anim(struct anim* animation);
void unload_anim(struct anim* animation);

#endif

