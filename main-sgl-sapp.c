#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gl.h"

#define JCW_WIDTH 80
#define JCW_HEIGHT 80
#define JCW_LIVE_CELLS 40
#include "jcw.h"

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

struct timespec req = {0, 100*1000000}; // 0.1 sec

static struct {
  sg_pass_action pass_action;
} state;

static void init(void) {
  sg_setup(&(sg_desc) {
    .context = sapp_sgcontext()
  });

  sgl_setup(&(sgl_desc_t) {
    .sample_count = sapp_sample_count()
  });

  state.pass_action = (sg_pass_action) {
    .colors[0] = {
      .action = SG_ACTION_CLEAR,
      .val = {
        0.0f, 0.0f, 0.0f, 1.0f
      }
    }
  };

  JCW_randomize();
  JCW_initBoard();
}

static void draw_quad(void) {
  sgl_defaults();
  sgl_begin_quads();
    sgl_v2f_c3b(-1.0f, -1.0f, 57, 255, 20);
    sgl_v2f_c3b( 1.0f, -1.0f, 57, 255, 20);
    sgl_v2f_c3b( 1.0f,  1.0f, 57, 255, 20);
    sgl_v2f_c3b(-1.0f,  1.0f, 57, 255, 20);
  sgl_end();
}

static void cleanup(void) {
  sgl_shutdown();
  sg_shutdown();
}

static void frame(void) {

  JCW_nextGeneration();

  for (int i = 1; i < JCW_HEIGHT+1; i++) {
    for (int j = 1; j < JCW_WIDTH+1; j++) {
      if (JCW_board[i][j] == 1) {
        sgl_viewport((j-1)*TILE_WIDTH, (i-1)*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, true);
        draw_quad();
      }
    }
  }
  // sgl_viewport(0, 0, w, h, true);

  sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
  sgl_draw();
  sg_end_pass();
  sg_commit();

  if (nanosleep(&req, NULL) == -1) {
    perror("ERROR: ");
    cleanup();
    exit(-1);
  }
}

sapp_desc sokol_main(int argc, char* argv[]) {
  (void)argc; (void)argv;
  return (sapp_desc){
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .width = TILE_WIDTH*JCW_WIDTH,
    .height = TILE_HEIGHT*JCW_HEIGHT,
    .window_title = "jcw (sgl)",
  };
}
