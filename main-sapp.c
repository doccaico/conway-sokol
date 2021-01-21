#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// 4:3 (800x600)
#define JCW_WIDTH 100
#define JCW_HEIGHT 75
#define JCW_LIVE_CELLS (JCW_HEIGHT / 2)
#include "jcw.h"

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

struct timespec req = {0, 100*1000000}; // 0.1 sec

static struct {
  sg_pass_action pass_action;
  sg_pipeline pip;
  sg_bindings bind;
} state;

void init(void) {
  sg_setup(&(sg_desc){
    .context = sapp_sgcontext()
  });

  float vertices[] = {
    // positions            colors
    -1.0f,  1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,
    1.0f,  1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,
  };

  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
    .size = sizeof(vertices),
    .content = vertices,
  });

  /* an index buffer with 2 triangles */
  uint16_t indices[] = {0, 1, 2,  0, 2, 3};
  state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc){
    .type = SG_BUFFERTYPE_INDEXBUFFER,
    .size = sizeof(indices),
    .content = indices,
  });

  const char* vs_src = 0;
  const char* fs_src = 0;
  switch (sg_query_backend()) {
    case SG_BACKEND_GLCORE33:
      vs_src =
        "#version 330\n"
        "in vec4 position;\n"
        "in vec4 color0;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "  gl_Position = position;\n"
        "  color = color0;\n"
        "}\n";
      fs_src =
        "#version 330\n"
        "in vec4 color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "  frag_color = color;\n"
        "}\n";
      break;
    case SG_BACKEND_GLES2:
      vs_src =
        "attribute vec4 position;\n"
        "attribute vec4 color0;\n"
        "varying vec4 color;\n"
        "void main() {\n"
        "  gl_Position = position;\n"
        "  color = color0;\n"
        "}\n";
      fs_src =
        "precision mediump float;\n"
        "varying vec4 color;\n"
        "void main() {\n"
        "  gl_FragColor = color;\n"
        "}\n";
      break;
    default:
      assert(false);
  }

  /* a shader (use separate shader sources here */
  sg_shader shd = sg_make_shader(&(sg_shader_desc) {
    .attrs = {
      [0].name = "position",
      [1].name = "color0",
    },
    .vs.source = vs_src,
    .fs.source = fs_src,
    }
  );

  /* a pipeline state object */
  state.pip = sg_make_pipeline(&(sg_pipeline_desc) {
    .shader = shd,
    .index_type = SG_INDEXTYPE_UINT16,
    .layout = {
      .attrs = {
        [0].format = SG_VERTEXFORMAT_FLOAT3,
        [1].format = SG_VERTEXFORMAT_FLOAT4,
      }
    },
  });

  /* default pass action */
  state.pass_action = (sg_pass_action) {
    .colors[0] = {
      .action=SG_ACTION_CLEAR,
      .val = {
        0.0f, 0.0f, 0.0f, 1.0f
      }
    }
  };

  JCW_randomize();
  JCW_initBoard();
}

void cleanup(void) {
  sg_shutdown();
}

static void gfx_adjust_viewport(int i, int j, int canvas_width, int canvas_height) {
  int px = 0;
  int xs = 0;
  int ys = 0;
  if (canvas_width < 300) {
    px = 2;
  } else if (canvas_width < 400) {
    px = 3;
  } else if (canvas_width < 500) {
    px = 4;
  } else if (canvas_width < 600) {
    px = 5;
  } else if (canvas_width < 700) {
    px = 6;
  } else if (canvas_width < 800) {
    px = 7;
  } else if (canvas_width < 900) {
    // desktop default(800x600)
    px = 8;
  } else {
    px = 9;
  }

  xs = (canvas_width - px * JCW_WIDTH) / 2;
  ys = (canvas_height - px * JCW_HEIGHT) / 2;

  sg_apply_viewport(
      xs+(j-1)*px, ys+(i-1)*px,
      px, px, true);
}

void frame(void) {

  const int canvas_width = sapp_width();
  const int canvas_height = sapp_height();

  JCW_nextGeneration();

  sg_begin_default_pass(&state.pass_action, canvas_width, canvas_height);
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);

  for (int i = 1; i < JCW_HEIGHT+1; i++) {
    for (int j = 1; j < JCW_WIDTH+1; j++) {
      if (JCW_board[i][j] == 1) {
        gfx_adjust_viewport(i, j, canvas_width, canvas_height);
        sg_draw(0, 6, 1);
      }
    }
  }

  sg_end_pass();
  sg_commit();

  if (nanosleep(&req, NULL) == -1) {
    assert(false);
  }
}

sapp_desc sokol_main(int argc, char* argv[]) {
  (void)argc; (void)argv;
  return (sapp_desc){
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .width = 800,
    .height = 600,
    .window_title = "jcw (sapp)",
  };
}
