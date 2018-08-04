#include "camera.h"
#include "detail.h"
#include <glm/gtx/transform.hpp>

auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	float elapsed, 
	float sensitivity) -> glm::vec3
{
	using detail::up;

	glm::vec3 res = direction;

	float x_angle = glm::radians(-cursor_dif.x) * sensitivity;// *elapsed;
	float y_angle = glm::radians(-cursor_dif.y) * sensitivity;// *elapsed;
	res = glm::mat3(glm::rotate(x_angle, up)) * res;
	glm::vec3 rotate_y = glm::cross(res, up);
	res = glm::mat3(glm::rotate(y_angle, rotate_y)) * res;

	return res;
}

camera::camera(glm::vec2 const & cursor_xy)
	: bound_entity(-1), cursor_ref(cursor_xy),
	  camera_view_matrix(1), entity::entity(glm::vec3(-1, 6, 1), glm::vec3(1, 0.1, 0.01), 1)
{
}

auto camera::bind(int32_t entity) -> void
{
	bound_entity = entity;
}

auto camera::look_at(glm::vec2 const & cursor, float elapsed, float sensitivity) -> void
{
	using detail::up;

	glm::vec2 diff = cursor - cursor_ref;
	cursor_ref = cursor;
	direction = look_fps(diff, direction, elapsed, sensitivity);
}

auto camera::update_view_matrix(void) -> glm::mat4 &
{
	using detail::up;
	return (camera_view_matrix = glm::lookAt(position, position + direction, up));
}

auto invert_matrix(camera & cam)->glm::mat4
{
	using detail::up;
	glm::vec3 inverted_position = cam.pos();
	glm::vec3 inverted_direction = cam.dir();
	inverted_position.y *= -1;
	inverted_direction.y *= -1;

	return glm::lookAt(inverted_position, inverted_position + inverted_direction, up);
}