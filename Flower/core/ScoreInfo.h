//
//  playerinfo.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_playerinfo_h
#define Sora_iPhone_playerinfo_h

#include <vector>

namespace flower {
    
    struct ScoreInfo {
      
        typedef std::vector<int32> ScoreList;
        ScoreList mScores;
        
        std::string mPlayerId;
        
    };
    
} // namespace flower

#endif
