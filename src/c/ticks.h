// From https://github.com/NiVZ78/Edge_Ticks_4
// Edge_Ticks_4 version 1.0.0 by Paul Niven

#include <pebble.h>

#define MAJOR_TICK_LENGTH   PBL_IF_ROUND_ELSE(28, 23)
#define MAJOR_TICK_WIDTH    3
#define MINOR_TICK_LENGTH   PBL_IF_ROUND_ELSE(10, 6)
#define MINOR_TICK_WIDTH    1



// courtesy of @robisodd
int32_t abs32(int32_t a) {return (a^(a>>31)) - (a>>31);}     // returns absolute value of A (only works on 32bit signed)

// courtesy of @robisodd
GPoint getPointOnRect(GRect r, int angle) {
  int32_t sin = sin_lookup(angle), cos = cos_lookup(angle);  // Calculate once and store, to make quicker and cleaner
  int32_t dy = sin>0 ? (r.size.h/2) : (0-r.size.h)/2;        // Distance to top or bottom edge (from center)
  int32_t dx = cos>0 ? (r.size.w/2) : (0-r.size.w)/2;        // Distance to left or right edge (from center)
  if(abs32(dx*sin) < abs32(dy*cos)) {                        // if (distance to vertical line) < (distance to horizontal line)
    dy = (dx * sin) / cos;                                   // calculate distance to vertical line
  } else {                                                   // else: (distance to top or bottom edge) < (distance to left or right edge)
    dx = (dy * cos) / sin;                                   // move to top or bottom line
  }
  return GPoint(dx+r.origin.x+(r.size.w/2), dy+r.origin.y+(r.size.h/2));  // Return point on rectangle
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//                                                TICK MARKS DEFINITIONS
//----------------------------------------------------------------------------------------------------------------------------------------------

//From https://gist.github.com/reciprocum/4e137664064194dac81af40f6a21c643
//Tick.c by @reciprocum

typedef enum { TICK_MINUTE
             , TICK_HOUR
             , TICK_QUARTER  
             }
TickType ;
              

static
TickType tickTypeMap[60]
= { TICK_QUARTER      // 0
  , TICK_MINUTE       // 1
  , TICK_MINUTE       // 2
  , TICK_MINUTE       // 3
  , TICK_MINUTE       // 4
  , TICK_HOUR         // 5
  , TICK_MINUTE       // 6
  , TICK_MINUTE       // 7
  , TICK_MINUTE       // 8
  , TICK_MINUTE       // 9
  , TICK_HOUR         // 10
  , TICK_MINUTE       // 11
  , TICK_MINUTE       // 12
  , TICK_MINUTE       // 13
  , TICK_MINUTE       // 14
  , TICK_QUARTER      // 15
  , TICK_MINUTE       // 16
  , TICK_MINUTE       // 17
  , TICK_MINUTE       // 18
  , TICK_MINUTE       // 19
  , TICK_HOUR         // 20
  , TICK_MINUTE       // 21
  , TICK_MINUTE       // 22
  , TICK_MINUTE       // 23
  , TICK_MINUTE       // 24
  , TICK_HOUR         // 25
  , TICK_MINUTE       // 26
  , TICK_MINUTE       // 27
  , TICK_MINUTE       // 28
  , TICK_MINUTE       // 29
  , TICK_QUARTER      // 30
  , TICK_MINUTE       // 31
  , TICK_MINUTE       // 32
  , TICK_MINUTE       // 33
  , TICK_MINUTE       // 34
  , TICK_HOUR         // 35
  , TICK_MINUTE       // 36
  , TICK_MINUTE       // 37
  , TICK_MINUTE       // 38
  , TICK_MINUTE       // 39
  , TICK_HOUR         // 40
  , TICK_MINUTE       // 41
  , TICK_MINUTE       // 42
  , TICK_MINUTE       // 43
  , TICK_MINUTE       // 44
  , TICK_QUARTER      // 45
  , TICK_MINUTE       // 46
  , TICK_MINUTE       // 47
  , TICK_MINUTE       // 48
  , TICK_MINUTE       // 49
  , TICK_HOUR         // 50
  , TICK_MINUTE       // 51
  , TICK_MINUTE       // 52
  , TICK_MINUTE       // 53
  , TICK_MINUTE       // 54
  , TICK_HOUR         // 55
  , TICK_MINUTE       // 56
  , TICK_MINUTE       // 57
  , TICK_MINUTE       // 58
  , TICK_MINUTE       // 59
  }
;

/*
void
update_proc( )
{

  for (int tickIdx = 0  ;  tickIdx < 60  ;  ++tickIdx)
    switch (tickTypeMap[tickIdx])
    {
      case TICK_MINUTE:
          } 
      break ;
    
      case TICK_HOUR:
          } 
      break ;
    
      case TICK_QUARTER:
        }
      break ;
    
      default :
      break ;
    }
}
*/

