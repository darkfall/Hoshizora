/*
 *  SoraMovieSprite.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MOVIE_SPRITE_H_
#define SORA_MOVIE_SPRITE_H_

#include "SoraSprite.h"
#include "SoraMoviePlayer.h"

namespace sora {
	
	class SoraMovieSprite: public SoraSprite {
	public:
		SoraMovieSprite(SoraMoviePlayer* moviePlayer);
		~SoraMovieSprite();
		
		uint32 update(float32 dt);
		
	private:
		SoraMoviePlayer* mMoviePlayer;
	};
	
	
} // namespace sora


#endif // SORA_MOVIE_SPRITE_H_