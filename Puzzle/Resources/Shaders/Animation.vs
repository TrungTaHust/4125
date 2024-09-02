attribute 	vec3 	a_posL;
attribute 	vec2 	a_uv;

varying 	vec2 	v_uv;

uniform		mat4	u_wvp;
uniform		float		u_currentFrame;
uniform		float		u_numFrames;
uniform		float		u_currentAction;
uniform		float		u_numActions;


void main()
{
	gl_Position = u_wvp * vec4(a_posL, 1.0);

	v_uv = vec2 (a_uv.x*(1.0/u_numFrames)+(1.0/u_numFrames)*u_currentFrame, a_uv.y*(1.0/u_numActions)+(1.0/u_numActions)*u_currentAction);
}
   