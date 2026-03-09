#pragma once

namespace smpl::ECS
{
    class SceneManager
    {
    public:
        SceneManager();
        ~SceneManager();

        bool init();
        bool load();
        bool uload();

        void update();
        void draw();

        void addScene();
        void removeScene();
        void getScene();
    };
}
