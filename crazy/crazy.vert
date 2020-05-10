#version 430

layout (location=0) in vec3 position;

// model parameters
//uniform vec3 trans;
uniform vec3 scale;
uniform vec3 theta;

// camera parameters
uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

// projection parameters
uniform float top;
uniform float bottom;
uniform float left;
uniform float right;
uniform float near;
uniform float far;

uniform float tf;

out vec4 varyingColor;


mat4 transMat(float x, float y, float z)
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0
	);
}

mat4 scaleMat()
{
	return mat4(
		scale.x, 0.0, 0.0, 0.0,
		0.0, scale.y, 0.0, 0.0,
		0.0, 0.0, scale.z, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rxMat(vec3 c, vec3 s)
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, c.x, s.x, 0.0,
		0.0, -s.x, c.x, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 ryMat(vec3 c, vec3 s)
{
	return mat4(
		c.y, 0.0, -s.y, 0.0,
		0.0, 1.0, 0.0, 0.0,
		s.y, 0.0, c.y, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rzMat(vec3 c, vec3 s)
{
	return mat4(
		c.z,  s.z,  0.0,  0.0,
        -s.z, c.z,  0.0,  0.0,
        0.0,  0.0,  1.0,  0.0,
        0.0,  0.0,  0.0,  1.0 
	);
}

mat4 viewMat()
{
	vec3 n = normalize( cPosition - cLookAt );
	vec3 u = normalize( cross(normalize(cUp), n) );
	vec3 v = normalize(cross(n,u));
	return mat4(
		u.x, v.x, n.x, 0.0,
		u.y, v.y, n.y, 0.0,
		u.z, v.z, n.z, 0.0,
		-dot(u, cPosition),
		-dot(v, cPosition),
		-dot(n, cPosition), 1.0
	);
}

mat4 frustumProjMat()
{
	return mat4(
		(2.0*near)/(right-left), 0.0, 0.0, 0.0,
		0.0, (2.0*near)/(top-bottom), 0.0, 0.0,
		(right+left)/(right-left),
		(top+bottom)/(top-bottom),
		-(far+near)/(far-near), -1.0,
		0.0, 0.0, (-2.0*far*near)/(far-near), 0.0
	);
}

mat4 orthoProjMat()
{
	return mat4(
		2.0/(right-left), 0.0, 0.0, 0.0,
		0.0, 2.0/(top-bottom), 0.0, 0.0,
		0.0, 0.0, -2.0/(far-near), 0.0,
		-(right+left)/(right-left),
		-(top+bottom)/(top-bottom),
		-(far+near)/(far-near), 1.0
	);
}

void main(void)
{ 
	float i = gl_InstanceID + tf;
	float a = tan(2.0*i) * 8.0;
	float b = cos(3.0*i) * 8.0;
	float c = cos(4.0*i) * 8.0;

	vec3 cosine = cos(vec3(theta.x*i, theta.y*i, theta.z*i));
	vec3 sine = sin(vec3(theta.x*i, theta.y*i, theta.z*i));

	mat4 modelMat = transMat(a,b,c) * rxMat(cosine, sine) * transMat(c,b,b) * ryMat(cosine,sine) * scaleMat();
	mat4 viewModelMat = viewMat() * modelMat;

	gl_Position = frustumProjMat() * viewModelMat * vec4(position,1.0);
	varyingColor = vec4(position,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

