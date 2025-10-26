#include <Camera/Camera.hpp>

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace smpl 
{
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const Projection projection_mode)
        : m_projection_mode( projection_mode )
        , m_position       ( position )
        , m_rotation       ( rotation )
        , m_far_clip_plane ( 100.f )
        , m_near_clip_plane(  0.1f )
        , m_viewport_width (2560.f )
        , m_viewport_height(1600.f )
        , m_field_of_view  (  60.f )
        , m_zoom           (  30.f )
        , m_update_view_matrix (false)
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

    const Camera::Projection& Camera::getProjectionMode() const
    {
        return m_projection_mode;
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

    const float Camera::getZoom() const
    {
        return m_zoom;
    }

    void Camera::updateViewMatrix()
    {
        if (m_projection_mode == Projection::Isometric)
        {
            const float pitch = glm::radians(m_rotation.x);
            const float yaw   = glm::radians(m_rotation.y);

            glm::mat4 yaw_mat   = glm::rotate(glm::mat4(1.0f), yaw, m_world_up);
            glm::mat3 pitch_mat = glm::rotate(yaw_mat, pitch, m_world_right);

            m_direction        = glm::normalize(glm::vec3(pitch_mat * glm::vec4(m_world_forward, 0.0f)));
            m_right            = glm::normalize(glm::vec3(yaw_mat   * glm::vec4(m_world_right,   0.0f)));
            m_up               = glm::cross(m_right, m_direction);
            m_view_matrix      = glm::lookAt(m_position, m_position + m_direction, m_up);
        }
        else
        {
            const float pitch = glm::radians(m_rotation.x);
            const float yaw   = glm::radians(m_rotation.y);

            glm::vec3 direction;
            direction.x = cos(yaw) * cos(pitch);
            direction.y = sin(yaw) * cos(pitch);
            direction.z = sin(pitch);

            m_direction   = glm::normalize(direction);
            m_right       = glm::normalize(glm::cross(m_direction, m_world_up));
            m_up          = glm::normalize(glm::cross(m_right, m_direction));
            m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
        }
    }

    void Camera::updateProjectionMatrix()
    {
        if (m_projection_mode == Projection::Perspective)
        {
            m_projection_matrix = glm::perspective(glm::radians(m_field_of_view), m_viewport_width / m_viewport_height, m_near_clip_plane, m_far_clip_plane);
        }
        else if (m_projection_mode == Projection::Isometric)
        {
            float aspect = m_viewport_width / m_viewport_height;
            m_projection_matrix = glm::ortho(-m_zoom * aspect, m_zoom * aspect,-m_zoom, m_zoom,m_near_clip_plane,m_far_clip_plane );
        }
        else
        {
            float aspect = m_viewport_width / m_viewport_height;
            m_projection_matrix = glm::ortho(-m_zoom * aspect, m_zoom * aspect, -m_zoom, m_zoom, m_near_clip_plane, m_far_clip_plane);
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

    void Camera::setZoom(float zoom)
    {
        m_zoom = zoom;
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