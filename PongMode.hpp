#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

/*
 * PongMode is a game mode that implements a single-player game of Pong.
 */

struct PongMode : Mode {
	PongMode();
	virtual ~PongMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	glm::vec2 court_radius = glm::vec2(7.0f, 5.0f);
	const int max_points = 5;
	int health = max_points;
	int points = 0;
	bool game_over = false;

	// ******** My definitions ********* // 
	glm::vec2 robot = glm::vec2(0.0f, 0.0f);
	glm::vec2 robot_velocity = glm::vec2(0.0f, 0.0f);
	const glm::vec2 robot_radius = glm::vec2(0.2f, 0.2f);
	const float robot_speed = 8	;
	bool robot_has_box = false; // is the robot carrying a box?
	int carried_box_color = 0; // color of the box the robot is carrying

	const glm::vec2 conveyor_radius = glm::vec2(0.5f, court_radius.y);
	const float conveyor_offset = 1.0f;
	const glm::vec2 conveyor = glm::vec2(-court_radius.x + conveyor_radius.x + conveyor_offset, 0.0f);
	const float conveyor_speed = 2.0f;
	const glm::vec2 conveyor_hole_radius = glm::vec2(0.3f, 0.3f);
	const glm::vec2 conveyor_hole = glm::vec2(-court_radius.x + conveyor_radius.x + conveyor_offset, -court_radius.y + conveyor_offset);

	const glm::vec2 box_radius = glm::vec2(0.1f, 0.1f);
	std::vector< glm::vec3> boxes = { glm::vec3(-court_radius.x+conveyor_radius.x+conveyor_offset, court_radius.y,0)};
	float new_box_update = 1.0f;
	const float boost_period = 5.0f;
	float boost_time = 5.0f;
	bool has_boost = false;
	const float conveyor_speedup_period = 2.0f;
	float conveyor_speedup_time = 2.0f;
	bool conveyor_speedup = false;

	const std::vector<glm::vec3> buckets = { glm::vec3(court_radius.x - 3.0f, 0.0f,0) ,
		glm::vec3(court_radius.x - 3.0f, 3.0f,1),  glm::vec3(court_radius.x - 3.0f, -3.0f,2) };
	const glm::vec2 bucket_radius = glm::vec2(0.5f, 0.5f);

	// ********************************* // 

	//----- pretty rainbow trails -----

	float trail_length = 1.3f;
	std::deque< glm::vec3 > ball_trail; //stores (x,y,age), oldest elements first

	//----- opengl assets / helpers ------

	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "PongMode::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};
