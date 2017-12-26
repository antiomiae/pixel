/*
* Copyright (c) 2008-2009 Erin Catto http://www.box2d.org
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

#ifndef ONE_SIDED_PLATFORM_HPP
#define ONE_SIDED_PLATFORM_HPP

class OneSidedPlatform : public Test
{
public:

	enum State
	{
		e_unknown,
		e_above,
		e_below
	};

	OneSidedPlatform()
	{
		// Ground
		{
			b2::BodyDef bd;
			b2::Body* ground = m_world->CreateBody(&bd);

			b2::EdgeShape shape;
			shape.Set(b2::Vec2(-20.0f, 0.0f), b2::Vec2(20.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		// Platform
		{
			b2::BodyDef bd;
			bd.position.Set(0.0f, 10.0f);
			b2::Body* body = m_world->CreateBody(&bd);

			b2::PolygonShape shape;
			shape.SetAsBox(3.0f, 0.5f);
			m_platform = body->CreateFixture(&shape, 0.0f);

			m_bottom = 10.0f - 0.5f;
			m_top = 10.0f + 0.5f;
		}

		// Actor
		{
			b2::BodyDef bd;
			bd.type = b2::dynamicBody;
			bd.position.Set(0.0f, 12.0f);
			b2::Body* body = m_world->CreateBody(&bd);

			m_radius = 0.5f;
			b2::CircleShape shape;
			shape.m_radius = m_radius;
			m_character = body->CreateFixture(&shape, 20.0f);

			body->SetLinearVelocity(b2::Vec2(0.0f, -50.0f));

			m_state = e_unknown;
		}
	}

	void PreSolve(b2::Contact* contact, const b2::Manifold* oldManifold)
	{
		Test::PreSolve(contact, oldManifold);

		b2::Fixture* fixtureA = contact->GetFixtureA();
		b2::Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA != m_platform && fixtureA != m_character)
		{
			return;
		}

		if (fixtureB != m_platform && fixtureB != m_character)
		{
			return;
		}

#if 1
		b2::Vec2 position = m_character->GetBody()->GetPosition();

		if (position.y < m_top + m_radius - 3.0f * b2::linearSlop)
		{
			contact->SetEnabled(false);
		}
#else
        b2::Vec2 v = m_character->GetBody()->GetLinearVelocity();
        if (v.y > 0.0f)
		{
            contact->SetEnabled(false);
        }
#endif
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Press: (c) create a shape, (d) destroy a shape.");
		m_textLine += DRAW_STRING_NEW_LINE;

        b2::Vec2 v = m_character->GetBody()->GetLinearVelocity();
        g_debugDraw.DrawString(5, m_textLine, "Character Linear Velocity: %f", v.y);
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new OneSidedPlatform;
	}

	b2::float32 m_radius, m_top, m_bottom;
	State m_state;
	b2::Fixture* m_platform;
	b2::Fixture* m_character;
};

#endif
