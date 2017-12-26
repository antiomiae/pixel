/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef VARYING_FRICTION_HPP
#define VARYING_FRICTION_HPP

class VaryingFriction : public Test
{
public:

	VaryingFriction()
	{
		{
			b2::BodyDef bd;
			b2::Body* ground = m_world->CreateBody(&bd);

			b2::EdgeShape shape;
			shape.Set(b2::Vec2(-40.0f, 0.0f), b2::Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(13.0f, 0.25f);

			b2::BodyDef bd;
			bd.position.Set(-4.0f, 22.0f);
			bd.angle = -0.25f;

			b2::Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(0.25f, 1.0f);

			b2::BodyDef bd;
			bd.position.Set(10.5f, 19.0f);

			b2::Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(13.0f, 0.25f);

			b2::BodyDef bd;
			bd.position.Set(4.0f, 14.0f);
			bd.angle = 0.25f;

			b2::Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(0.25f, 1.0f);

			b2::BodyDef bd;
			bd.position.Set(-10.5f, 11.0f);

			b2::Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(13.0f, 0.25f);

			b2::BodyDef bd;
			bd.position.Set(-4.0f, 6.0f);
			bd.angle = -0.25f;

			b2::Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2::PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);

			b2::FixtureDef fd;
			fd.shape = &shape;
			fd.density = 25.0f;

			float friction[5] = {0.75f, 0.5f, 0.35f, 0.1f, 0.0f};

			for (int i = 0; i < 5; ++i)
			{
				b2::BodyDef bd;
				bd.type = b2::dynamicBody;
				bd.position.Set(-15.0f + 4.0f * i, 28.0f);
				b2::Body* body = m_world->CreateBody(&bd);

				fd.friction = friction[i];
				body->CreateFixture(&fd);
			}
		}
	}

	static Test* Create()
	{
		return new VaryingFriction;
	}
};

#endif
