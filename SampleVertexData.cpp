

// *******************
// VERTEX BUFFER DATA:
// *******************

#ifdef DEFINITION
struct vertex
{
	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3	color;
	glm::vec2	texCoord;
};
#endif

struct vertex VertexData[] =
{
	// triangle 0-2-3:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{  0.,  0., -1. },
		{  1.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #2:
	{
		{ -1.,  1., -1. },
		{  0.,  0., -1. },
		{  0.,  1.,  0. },
		{  0., 1. }
	},

	// vertex #3:
	{
		{  1.,  1., -1. },
		{  0.,  0., -1. },
		{  0.,  1.,  1. },
		{  1., 1. }
	},

	// triangle 4-5-7:
	// vertex #4:
	{
		{ -1., -1.,  -1. },
		{  0.,  0.,  1. },
		{  1.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #5:
	{
		{  1., -1.,  -1. },
		{  0.,  0.,  1. },
		{  1.,  0.,  1. },
		{  1., 0. }
	},

	// vertex #7:
	{
		{  1.,  1.,  -1. },
		{  0.,  0.,  1. },
		{  1.,  1.,  1. },
		{  1., 1. }
	},
};
/*
	// triangle 0-3-1:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{  0.,  0., -1. },
		{  0.,  0.,  0. },
		{  1., 0. }
	},

	// vertex #3:
	{
		{  1.,  1., -1. },
		{  0.,  0., -1. },
		{  0.,  1.,  1. },
		{  0., 1. }
	},

	// vertex #1:
	{
		{  1., -1., -1. },
		{  0.,  0., -1. },
		{  0.,  0.,  1. },
		{  0., 0. }
	},


	// triangle 4-5-7:
	// vertex #4:
	{
		{ -1., -1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #5:
	{
		{  1., -1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  0.,  1. },
		{  1., 0. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  1.,  1. },
		{  1., 1. }
	},


	// triangle 4-7-6:
	// vertex #4:
	{
		{ -1., -1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  1.,  1. },
		{  1., 1. }
	},

	// vertex #6:
	{
		{ -1.,  1.,  1. },
		{  0.,  0.,  1. },
		{  1.,  1.,  0. },
		{  0., 1. }
	},


	// triangle 1-3-7:
	// vertex #1:
	{
		{  1., -1., -1. },
		{  1.,  0.,  0. },
		{  0.,  0.,  1. },
		{  1., 0. }
	},

	// vertex #3:
	{
		{  1.,  1., -1. },
		{  1.,  0.,  0. },
		{  0.,  1.,  1. },
		{  1., 1. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  1.,  0.,  0. },
		{  1.,  1.,  1. },
		{  0., 1. }
	},


	// triangle 1-7-5:
	// vertex #1:
	{
		{  1., -1., -1. },
		{  1.,  0.,  0. },
		{  0.,  0.,  1. },
		{  1., 0. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  1.,  0.,  0. },
		{  1.,  1.,  1. },
		{  0., 1. }
	},

	// vertex #5:
	{
		{  1., -1.,  1. },
		{  1.,  0.,  0. },
		{  1.,  0.,  1. },
		{  0., 0. }
	},


	// triangle 0-4-6:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{ -1.,  0.,  0. },
		{  0.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #4:
	{
		{ -1., -1.,  1. },
		{ -1.,  0.,  0. },
		{  1.,  0.,  0. },
		{  1., 0. }
	},

	// vertex #6:
	{
		{ -1.,  1.,  1. },
		{ -1.,  0.,  0. },
		{  1.,  1.,  0. },
		{  1., 1. }
	},


	// triangle 0-6-2:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{ -1.,  0.,  0. },
		{  0.,  0.,  0. },
		{  0., 0. }
	},

	// vertex #6:
	{
		{ -1.,  1.,  1. },
		{ -1.,  0.,  0. },
		{  1.,  1.,  0. },
		{  1., 1. }
	},

	// vertex #2:
	{
		{ -1.,  1., -1. },
		{ -1.,  0.,  0. },
		{  0.,  1.,  0. },
		{  0., 1. }
	},


	// triangle 2-6-7:
	// vertex #2:
	{
		{ -1.,  1., -1. },
		{  0.,  1.,  0. },
		{  0.,  1.,  0. },
		{  0., 1. }
	},

	// vertex #6:
	{
		{ -1.,  1.,  1. },
		{  0.,  1.,  0. },
		{  1.,  1.,  0. },
		{  0., 0. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  0.,  1.,  0. },
		{  1.,  1.,  1. },
		{  1., 0. }
	},


	// triangle 2-7-3:
	// vertex #2:
	{
		{ -1.,  1., -1. },
		{  0.,  1.,  0. },
		{  0.,  1.,  0. },
		{  0., 1. }
	},

	// vertex #7:
	{
		{  1.,  1.,  1. },
		{  0.,  1.,  0. },
		{  1.,  1.,  1. },
		{  1., 0. }
	},

	// vertex #3:
	{
		{  1.,  1., -1. },
		{  0.,  1.,  0. },
		{  0.,  1.,  1. },
		{  1., 1. }
	},


	// triangle 0-1-5:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{  0., -1.,  0. },
		{  0.,  0.,  0. },
		{  0.,  0. }
	},

	// vertex #1:
	{
		{  1., -1., -1. },
		{  0., -1.,  0. },
		{  0.,  0.,  1. },
		{  1.,  0. }
	},

	// vertex #5:
	{
		{  1., -1.,  1. },
		{  0., -1.,  0. },
		{  1.,  0.,  1. },
		{  1.,  1. }
	},


	// triangle 0-5-4:
	// vertex #0:
	{
		{ -1., -1., -1. },
		{  0., -1.,  0. },
		{  0.,  0.,  0. },
		{  0.,  0. }
	},

	// vertex #5:
	{
		{  1., -1.,  1. },
		{  0., -1.,  0. },
		{  1.,  0.,  1. },
		{  1.,  1. }
	},

	// vertex #4:
	{
		{ -1., -1.,  1. },
		{  0., -1.,  0. },
		{  1.,  0.,  0. },
		{  0.,  1. }
	},
};
*/

struct vertex JustVertexData[ ] =
{
	// vertex #0:
	{
		{ -1., -1., -1. },
		{  0.,  0., -1. },
		{  1.,  1.,  1. },
		{  1., 0. }
	},

	// vertex #1:
	{
		{  1., -1., -1. },
		{  0.,  0., -1. },
		{  1.,  1.,  1. },
		{  0., 0. }
	},

	// vertex #2:
	{
		{ -1.,  1., -1. },
		{  0.,  0., -1. },
		{  1.,  1.,  1. },
		{  1., 1. }
	}
};


int JustIndexData[ ] =
{
	0, 2, 3,
	0, 3, 1,
	4, 5, 7,
	4, 7, 6,
	1, 3, 7,
	1, 7, 5,
	0, 4, 6,
	0, 6, 2,
	2, 6, 7,
	2, 7, 3,
	0, 1, 5,
	0, 5, 4,
};
