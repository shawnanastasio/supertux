//  $Id: flyingsnowball.cpp 2642 2005-06-26 13:38:53Z matzebraun $
// 
//  SuperTux
//  Copyright (C) 2005 Matthias Braun <matze@braunis.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <config.h>
#include <stdio.h>

#include "spidermite.hpp"

static const float FLYTIME = 1.2;
static const float FLYSPEED = 100.0;

SpiderMite::SpiderMite(const lisp::Lisp& reader)
{
  reader.get("x", start_position.x);
  reader.get("y", start_position.y);
  bbox.set_size(31.8, 31.8);
  sprite = sprite_manager->create("images/creatures/spidermite/spidermite.sprite");
  physic.enable_gravity(false);
}

SpiderMite::SpiderMite(float pos_x, float pos_y)
{
  start_position.x = pos_x;
  start_position.y = pos_y;
  bbox.set_size(31.8, 31.8);
  sprite = sprite_manager->create("images/creatures/spidermite/spidermite.sprite");
  physic.enable_gravity(false);
}

void
SpiderMite::write(lisp::Writer& writer)
{
  writer.start_list("spidermite");

  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);

  writer.end_list("spidermite");
}

void 
SpiderMite::activate()
{
  sprite->set_action(dir == LEFT ? "left" : "right");
  mode = FLY_UP;
  physic.set_velocity_y(FLYSPEED);
  timer.start(FLYTIME/2);
}

bool
SpiderMite::collision_squished(Player& player)
{
  sprite->set_action(dir == LEFT ? "squished-left" : "squished-right");
  kill_squished(player);
  return true;
}

HitResponse
SpiderMite::collision_solid(GameObject& , const CollisionHit& hit)
{
  if(fabsf(hit.normal.y) > 1.5) { // hit floor or roof?
    physic.set_velocity_y(0);
  }

  return CONTINUE;
}

void
SpiderMite::active_update(float elapsed_time) 
{
  if(timer.check()) {
    if(mode == FLY_UP) {
      mode = FLY_DOWN;
      physic.set_velocity_y(-FLYSPEED);
    } else if(mode == FLY_DOWN) {
      mode = FLY_UP;
      physic.set_velocity_y(FLYSPEED);
    }
    timer.start(FLYTIME);
  }
  movement=physic.get_movement(elapsed_time);

  Player* player = this->get_nearest_player();
  if (player) {
    dir = (player->get_pos().x > get_pos().x) ? RIGHT : LEFT;
    sprite->set_action(dir == LEFT ? "left" : "right");
  }
}

IMPLEMENT_FACTORY(SpiderMite, "spidermite")
