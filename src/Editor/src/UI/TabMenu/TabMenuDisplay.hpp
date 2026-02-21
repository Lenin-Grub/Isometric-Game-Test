#pragma once

namespace Editor
{
    class TabMenuDisplay
    {
    public:
        TabMenuDisplay();
        virtual ~TabMenuDisplay() = default;

        void show();

        void showSettings();

    public:
        bool show_demo_window = false;
        bool m_show_settings = true;

    private:
    };
}