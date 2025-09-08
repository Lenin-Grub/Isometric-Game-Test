#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace smpl 
{
    class Camera
    {
    public:
        enum class Projection
        {
            Perspective,
            Orthographic,
            Isometric
        };

        Camera(const glm::vec3& position = { 0, 0, 0 },
               const glm::vec3& rotation = { 0, 0, 0 },
               const Projection projection_mode = Projection::Perspective);

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation);
        void setProjection(const Projection projection_mode);
        void setFarClipPlane(const float far);
        void setNearClipPlane(const float near);
        void setViewportSize(const float width, const float height);
        void setFieldOfView(const float fov);

        const glm::mat4& getViewMatrix();
        const glm::mat4& getProjectionMatrix() const;
        const float getFarClipPlane() const;
        const float getNearClipPlane() const;
        const float getFieldOfView() const;

        void moveForward(const float delta);
        void moveRight(const float delta);
        void moveUp(const float delta);

        const glm::vec3& getPosition() const;
        const glm::vec3& getRotation() const;

        // movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
        // rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
        void addMovementAndRotation(const glm::vec3& movement_delta,
                                    const glm::vec3& rotation_delta);

    private:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 m_position;
        glm::vec3 m_rotation; // X - Roll, Y - Pitch, Z - Yaw
        Projection m_projection_mode;

        glm::vec3 m_direction;
        glm::vec3 m_right;
        glm::vec3 m_up;
        float m_far_clip_plane{ 100.f };
        float m_near_clip_plane{ 0.1f };
        float m_viewport_width{ 800.f };
        float m_viewport_height{ 600.f };
        float m_field_of_view{ 60.f };

        static constexpr glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;
        bool m_update_view_matrix = false;
    };
}