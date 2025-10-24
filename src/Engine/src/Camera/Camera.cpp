#include <Camera/Camera.hpp>

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace smpl 
{
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const Projection projection_mode)
        : m_position(position)
        , m_rotation(rotation)
        , m_projection_mode(projection_mode)
    {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    const glm::mat4& Camera::getViewMatrix()
    {
        if (m_update_view_matrix)
        {
            updateViewMatrix();
            m_update_view_matrix = false;
        }
        return m_view_matrix;
    }

    const glm::mat4& Camera::getProjectionMatrix() const
    {
        return m_projection_matrix;
    }

    const float Camera::getFarClipPlane() const
    {
        return m_far_clip_plane;
    }

    const float Camera::getNearClipPlane() const
    {
        return m_near_clip_plane;
    }

    const float Camera::getFieldOfView() const
    {
        return m_field_of_view;
    }

    void Camera::updateViewMatrix()
    {
        const float roll_in_radians  = glm::radians(m_rotation.x);
        const float pitch_in_radians = glm::radians(m_rotation.y);
        const float yaw_in_radians   = glm::radians(m_rotation.z);

        glm::mat3 yaw_mat   = glm::rotate(glm::mat4(1), yaw_in_radians,   m_world_up);      // вокруг Z
        glm::mat3 pitch_mat = glm::rotate(glm::mat4(1), pitch_in_radians, m_world_right);   // вокруг X
        glm::mat3 roll_mat  = glm::rotate(glm::mat4(1), roll_in_radians,  m_world_forward); // вокруг Y

        glm::mat3 euler_rotate_matrix = roll_mat * pitch_mat * yaw_mat;

        m_direction                         = glm::normalize(euler_rotate_matrix * m_world_forward);
        m_right                             = glm::normalize(euler_rotate_matrix * m_world_right);
        m_up                                = glm::cross(m_right, m_direction);
        m_view_matrix                       = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    void Camera::updateProjectionMatrix()
    {
        if (m_projection_mode == Projection::Perspective)
        {
            m_projection_matrix = glm::perspective(glm::radians(m_field_of_view), m_viewport_width / m_viewport_height, m_near_clip_plane, m_far_clip_plane);
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
                                            0, 1 / t, 0, 0,
                                            0, 0, -2 / (f - n), 0,
                                            0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::setPosition(const glm::vec3& position)
    {
        m_position = position;
        m_update_view_matrix = true;
    }

    void Camera::setRotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        m_update_view_matrix = true;
    }

    void Camera::setPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation)
    {
        m_position = position;
        m_rotation = rotation;
        m_update_view_matrix = true;
    }

    void Camera::setProjection(const Projection projection_mode)
    {
        m_projection_mode = projection_mode;
        updateProjectionMatrix();
    }

    void Camera::setFarClipPlane(const float far)
    {
        m_far_clip_plane = far;
        updateProjectionMatrix();
    }

    void Camera::setNearClipPlane(const float near)
    {
        m_near_clip_plane = near;
        updateProjectionMatrix();
    }

    void Camera::setViewportSize(const float width, const float height)
    {
        m_viewport_width = width;
        m_viewport_height = height;
        updateProjectionMatrix();
    }

    void Camera::setFieldOfView(const float fov)
    {
        m_field_of_view = fov;
        updateProjectionMatrix();
    }

    void Camera::moveForward(const float delta)
    {
        m_position += m_direction * delta;
        m_update_view_matrix = true;
    }

    void Camera::moveRight(const float delta)
    {
        m_position += m_right * delta;
        m_update_view_matrix = true;
    }

    void Camera::moveUp(const float delta)
    {
        m_position += m_world_up * delta;
        m_update_view_matrix = true;
    }

    const glm::vec3& Camera::getPosition() const
    {
        return m_position;
    }

    const glm::vec3& Camera::getRotation() const
    {
        return m_rotation;
    }

    void Camera::addMovementAndRotation(const glm::vec3& movement_delta,
                                             const glm::vec3& rotation_delta)
    {
        m_position += m_direction * movement_delta.x;
        m_position += m_right     * movement_delta.y;
        m_position += m_up        * movement_delta.z;
        m_rotation += rotation_delta;
        m_update_view_matrix = true;
    }

}