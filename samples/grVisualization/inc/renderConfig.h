#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H



static const int TRAJECTORY_LENGTH = 40;


struct vec3f
{
        float x;
        float y;
        float z;
        vec3f( float new_x = 0, float new_y = 0, float new_z = 0)
        {
                x = new_x;
                y = new_y;
                z = new_z;
        }
};

struct rgba_color
{
          unsigned int r;
          unsigned int g;
          unsigned int b;
          unsigned int a;
          rgba_color( unsigned int new_r = 0, unsigned int new_g = 0, unsigned int new_b = 0, unsigned int alpha = 200)
          {
                  r = new_r;
                  g = new_g;
                  b = new_b;
                  a = alpha;
          }
};

struct arm_dimensions
{

        float length;
        float radius;
        float hand_width;
        float hand_height;
        float hand_depth;
        float phalange_radiuses[5] = {0.09f, 0.10f, 0.11f, 0.10f, 0.11f};
        float phalange_heights[5] = {0.3f, 0.35f, 0.4f, 0.35f, 0.35f};

        vec3f initial_pos;

        arm_dimensions(){
                length = 2.f;
                radius = 0.25f;
                hand_width = 1.1f;
                hand_height = 1.f;
                hand_depth = 0.2f;

                initial_pos = vec3f(2.0f, -2.0f, -10.f);
        };
};


struct screen_size
{
        int width;
        int height;
        screen_size()
        {
                width = 1920;
                height = 1080;
        }
};

struct render_data
{
        int fps;
        int cylinder_sides;
        float initial_depth;
        float scene_depth;
        screen_size screen;
        const arm_dimensions arm;
        size_t trajectory_length = 40;
        rgba_color linesColor;
        rgba_color planeColor;
        render_data()
        {

                linesColor = rgba_color(255, 0, 0, 255);
                planeColor = rgba_color(255, 255, 255, 255);
                fps = 60;
                cylinder_sides = 10;
                initial_depth = arm.initial_pos.z;
                scene_depth = -(arm.length + arm.hand_height + arm.phalange_radiuses[3]);
        }
};


#endif // RENDER_CONFIG_H
