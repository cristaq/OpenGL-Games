#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // TODO(student): Translate the camera using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            position += glm::normalize(forward) * distance;

        }

        void TranslateUpward(float distance)
        {
            // TODO(student): Translate the camera using the `up` vector.
            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            // TODO(student): See instructions below. Read the entire thing!
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.
            position += glm::normalize(right) * distance;

        }

        void RotateFirstPerson_OX(float angle)
        {
            // Compute the new "forward" and "up" vectors
            // Attention! Don't forget to normalize the vectors
            // Use glm::rotate()
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            // Compute the new "forward", "up" and "right" vectors
            // Don't forget to normalize the vectors
            // Use glm::rotate()
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // Compute the new Right and Up, Forward stays the same
            // Don't forget to normalize the vectors
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
            forward = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            // Rotate the camera in Third Person mode - OX axis
            // Use distanceToTarget as translation distance
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // Rotate the camera in Third Person mode - OY axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // Rotate the camera in Third Person mode - OZ axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
