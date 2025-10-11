#include "../include/game/anim.h"

void init_anim(struct anim* animation, const char* path, int frame_delay){

    animation->gif_anim = LoadImageAnim(path, &animation->frame_count);
    animation->tex_anim = LoadTextureFromImage(animation->gif_anim);
    animation->next_frame_offset = 0;
    animation->current_frame = 0;
    animation->frame_delay = frame_delay;
    animation->frame_counter = 0;
}

void update_anim(struct anim* animation){

    animation->frame_counter++;
    if (animation->frame_counter >= animation->frame_delay) {
        animation->current_frame++;
        if (animation-> current_frame >= animation->frame_count) {
            animation->current_frame = 0;
        }

        animation->next_frame_offset =
         animation->gif_anim.width * animation->gif_anim.height * 4 * animation->current_frame;

        UpdateTexture(animation->tex_anim,
                      ((unsigned char*)animation->gif_anim.data +
                          animation->next_frame_offset));

        animation->frame_counter = 0;
    }
    
}

void unload_anim(struct anim* animation){
    UnloadTexture(animation->tex_anim);
    UnloadImage(animation->gif_anim);
}
