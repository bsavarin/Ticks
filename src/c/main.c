#include <pebble.h>	
#include "ticks.h"
#include "dithered_rects.h"

//----------------------------------------------------------------------------------------------------------------------------------------------
//                                           DEFINES (COLOUR/BT/BATTERY/CLOCK HANDS/HEALTH)
//----------------------------------------------------------------------------------------------------------------------------------------------

#define HOURS_RADIUS 3
#define INSET PBL_IF_ROUND_ELSE(5, 3)//5,3,4

static Window *window;
static Layer *window_layer;
static Layer *s_canvas;

//Change the value below for different tick mark views
//0 = Ticks, 1 = Main Ticks, 2 = Digits/Ticks, 3 = Less Ticks, 4 = Digits, 5 = None
static int tickMarks = 0;

static int bground_colour = 0x000000; // Black
static int fground_colour = 0xFFFFFF; // White

static GColor background_colour = {.argb = GColorBlackARGB8};
static GColor foreground_colour = {.argb = GColorWhiteARGB8};


static void layer_update_proc(Layer *layer, GContext *ctx) {
   GRect tick_layer_bounds = layer_get_bounds(layer);
    
    int halfW = tick_layer_bounds.size.w/2;
    int halfH = tick_layer_bounds.size.h/2;
    int originX = tick_layer_bounds.origin.x;
    int originY = tick_layer_bounds.origin.y;
  
    //"Bezel" background
    draw_dithered_rect(ctx, tick_layer_bounds, background_colour, foreground_colour, DITHER_50_PERCENT);
    int radius = 0;
    radius = PBL_IF_RECT_ELSE((TRIG_MAX_ANGLE * halfH) / sin_lookup(atan2_lookup(halfH, halfW)), halfW);
    GRect circle_bounds;
    circle_bounds = GRect((halfW-radius)+originX,(halfH-radius)+originY, radius*2, radius*2); 
  
  //Defines
  GPoint rect_point = GPoint(0,0);  // GPoint to store top left co-ord of rectangle
    
 //Clock tick marks
  
  graphics_context_set_fill_color(ctx, foreground_colour);
 
 for (int tickIdx = 0  ;  tickIdx < 60  ;  ++tickIdx)
    switch (tickTypeMap[tickIdx])
    { 
      case TICK_MINUTE: // Minutes ticks
          if(tickMarks == 0 || tickMarks == 2) {
          int iTick = tickIdx*6;
          graphics_fill_radial(ctx, circle_bounds, GOvalScaleModeFitCircle, PBL_IF_RECT_ELSE(radius, MINOR_TICK_LENGTH), DEG_TO_TRIGANGLE(iTick-1), DEG_TO_TRIGANGLE(iTick));
      break ;
   
      case TICK_HOUR: break ;
      case TICK_QUARTER:  break ;
     }
   
   #ifdef PBL_RECT
   // Draw another rect in the middle using border colour to hide the inner parts of the minute ticks
   draw_dithered_rect(ctx, grect_inset(tick_layer_bounds, GEdgeInsets(MINOR_TICK_LENGTH)), background_colour, foreground_colour, DITHER_50_PERCENT);
   #endif            
 } 
  
   for (int tickIdx = 0  ;  tickIdx < 60  ;  ++tickIdx)
    switch (tickTypeMap[tickIdx])
    {  
     case TICK_MINUTE: break ;
     
      case TICK_HOUR: // Hours ticks - 1, 2, 4, 5, 7, 8, 10, 11 o'clock
        if(tickMarks == 0 || tickMarks == 1 || tickMarks == 2 || tickMarks == 3) {  
        int iTick = tickIdx*6;
        graphics_fill_radial(ctx, circle_bounds, GOvalScaleModeFitCircle, PBL_IF_RECT_ELSE(radius, MAJOR_TICK_LENGTH), DEG_TO_TRIGANGLE(iTick-MINOR_TICK_WIDTH), DEG_TO_TRIGANGLE(iTick+MINOR_TICK_WIDTH));
      } 
      break ;
    
      case TICK_QUARTER: //Hours ticks - 3, 6, 9 & 12 o'clock
        if(tickMarks == 0 || tickMarks == 1) {
        int iTick = tickIdx*6;                              
            if (iTick == 0) {rect_point.x = halfW + originX; rect_point.y = originY;}
            if (iTick == 180) {rect_point.x = halfW; rect_point.y = halfH*2 - MAJOR_TICK_LENGTH + originY;}
              graphics_fill_rect(ctx, GRect(rect_point.x-MAJOR_TICK_WIDTH-1, rect_point.y, MAJOR_TICK_WIDTH, MAJOR_TICK_LENGTH), 0, GCornerNone);
              graphics_fill_rect(ctx, GRect(rect_point.x, rect_point.y, MAJOR_TICK_WIDTH, MAJOR_TICK_LENGTH), 0, GCornerNone); 
                     
            if (iTick == 90) {rect_point.x = halfW*2 - MAJOR_TICK_LENGTH + originX; rect_point.y = halfH + originX;}
            if (iTick == 270) {rect_point.x = originX; rect_point.y = halfH + originX;}
              graphics_fill_rect(ctx, GRect(rect_point.x, rect_point.y-MAJOR_TICK_WIDTH-1, MAJOR_TICK_LENGTH, MAJOR_TICK_WIDTH), 0, GCornerNone);
              graphics_fill_rect(ctx, GRect(rect_point.x, rect_point.y, MAJOR_TICK_LENGTH, MAJOR_TICK_WIDTH), 0, GCornerNone); 
        }
      break ;
    }
  
  // Fill the center with the background colour
      graphics_context_set_fill_color(ctx, background_colour);
      #ifdef PBL_ROUND
        graphics_fill_radial(ctx, grect_inset(tick_layer_bounds, GEdgeInsets(MAJOR_TICK_LENGTH)), GOvalScaleModeFitCircle, radius-MAJOR_TICK_LENGTH, DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(360));
      #else
        graphics_fill_rect(ctx, grect_inset(tick_layer_bounds, GEdgeInsets(MAJOR_TICK_LENGTH)), 0, GCornerNone);
      #endif
   
  // Digits
     // graphics_context_set_fill_color(ctx, foreground_colour);   
     if(tickMarks == 2 || tickMarks == 3 || tickMarks == 4) {      
 /*       #ifdef PBL_HEALTH
       // Digits - Basalt, Chalk, Diorite (and Emery - soon tm) only
        FContext fctx;
        fctx_init_context(&fctx, ctx);
        fctx_set_fill_color(&fctx, foreground_colour);
        fctx_set_text_size(&fctx, s_ffont_28, 28);
        fctx_begin_fill(&fctx);
       
        FPoint no_03F = FPointI(PBL_IF_ROUND_ELSE(164, 132),PBL_IF_ROUND_ELSE(90, 84));
        fctx_set_offset(&fctx, no_03F);
        fctx_draw_string(&fctx,"3", s_ffont_28, GTextAlignmentCenter, FTextAnchorMiddle);
 
        FPoint no_06F = FPointI(PBL_IF_ROUND_ELSE(90, 72),PBL_IF_ROUND_ELSE(163, 155));
        fctx_set_offset(&fctx, no_06F);
        fctx_draw_string(&fctx,"6", s_ffont_28, GTextAlignmentCenter, FTextAnchorMiddle);      
       
        FPoint no_09F = FPointI(PBL_IF_ROUND_ELSE(15, 12),PBL_IF_ROUND_ELSE(90, 84));
        fctx_set_offset(&fctx, no_09F);
        fctx_draw_string(&fctx,"9", s_ffont_28, GTextAlignmentCenter, FTextAnchorMiddle);      
       
        FPoint no_12F = FPointI(PBL_IF_ROUND_ELSE(90, 72),PBL_IF_ROUND_ELSE(15, 10));
        fctx_set_offset(&fctx, no_12F);
        fctx_draw_string(&fctx,"12", s_ffont_28, GTextAlignmentCenter, FTextAnchorMiddle);
       
        fctx_end_fill(&fctx);      
        fctx_deinit_context(&fctx);
        #else*/
       
       // Digits - Aplite only
       graphics_context_set_text_color(ctx, foreground_colour);
        graphics_draw_text(ctx,"3",fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),GRect(125,67,17,17),GTextOverflowModeTrailingEllipsis,GTextAlignmentCenter,NULL);
        graphics_draw_text(ctx,"6",fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),GRect(64,137,17,17),GTextOverflowModeTrailingEllipsis,GTextAlignmentCenter,NULL);
        graphics_draw_text(ctx,"9",fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),GRect(3,67,17,17),GTextOverflowModeTrailingEllipsis,GTextAlignmentCenter,NULL);
        graphics_draw_text(ctx,"12",fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),GRect(55,-7,35,27),GTextOverflowModeTrailingEllipsis,GTextAlignmentCenter,NULL);
     //   #endif
    } 

  //Circle around clock
    #ifdef PBL_ROUND
    GRect bounds = layer_get_bounds(layer);	
    GRect frame3 = grect_inset(bounds, GEdgeInsets(-(INSET * 15)/100));
    graphics_context_set_antialiased(ctx, true);
    graphics_context_set_fill_color(ctx, foreground_colour);
    graphics_fill_radial(ctx, frame3, GOvalScaleModeFitCircle, 2, DEG_TO_TRIGANGLE(0), DEG_TO_TRIGANGLE(360));
    #else
    #endif  
   
}

static void update_time() {
  // Get a tm structure
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {  
      update_time();    
}


static void window_load(Window *window) {
  window_layer = window_get_root_layer(window);
//  GRect bounds = layer_get_bounds(window_layer);
  
  GRect full_bounds = layer_get_bounds(window_layer); 
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(window_layer);
   
  s_canvas = layer_create(full_bounds);
  layer_set_update_proc(s_canvas, &layer_update_proc);
  layer_add_child(window_layer, s_canvas);  
  
}	


static void window_unload(Window *window) {    
  layer_destroy(s_canvas);
}	


static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
    
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  
}

static void deinit() {
  tick_timer_service_unsubscribe();
  
  window_destroy(window); 
}	

int main() {
  init();
  app_event_loop();
  deinit();
}	
  