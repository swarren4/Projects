#include "core/oxygine.h"
#include "Stage.h"
#include "DebugActor.h"
#include "gauntmore.h"
using namespace oxygine;


/**
 * The main loop method. Called each frame.
 */
int mainloop() {
	gauntmore_update();
	// Update our stage
	// Update all actors. Actor::update is called for all children.
	getStage()->update();
	
	if (core::beginRendering()) {
		Color clearColor(32, 32, 32, 255);
		Rect viewport(Point(0, 0), core::getDisplaySize());
		// Render all actors. Actor::render is called for all children.
		getStage()->render(clearColor, viewport);

		core::swapDisplayBuffers();
	}

	// Update internal components
	// All input events are passed to Stage::instance.handleEvent
	// If done is true then User requests quit from app.
	bool done = core::update();

	return done ? 1 : 0;
}


/** 
 * The application entry point.
 */
void run() {
	ObjectBase::__startTracingLeaks();

	// Initialize Oxygine's internal stuff.
	core::init_desc desc;
    
    // Setup initial window size.
    desc.w = 480;
    desc.h = 480;

	gauntmore_preinit();
	core::init(&desc);

	// Create the Stage. Stage is a root node.
	Stage::instance = new Stage(true);
	Point size = core::getDisplaySize();
	getStage()->setSize(size);

	// DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff.
    //DebugActor::show();
		
	// Initialize the game and attach to the stage.
	gauntmore_init();

	// Start the main loop.
	while (1) {
		int done = mainloop();
		if (done)
			break;
	}
	// If the user wants to leave application...

	// Dump all created objects into log. All created and not freed resources are displayed.
	ObjectBase::dumpCreatedObjects();

	// Cleanup everything and call ObjectBase::dumpObjects() again.
	// Free all allocated resources and delete all created actors.
	// All actors/sprites are smart pointer objects and they don't need to removed by hand
	// but this does it anyway just to be safe.

	gauntmore_destroy();

	// Releases all internal components and Stage
	core::release();

	// Dump list should be empty now.
	// Everything has been deleted and there can't be any memory leaks.
	ObjectBase::dumpCreatedObjects();

	ObjectBase::__stopTracingLeaks();
}

// Main methods depending on context:

#ifdef __S3E__
int main(int argc, char* argv[]) {
	run();
	return 0;
}
#endif


#ifdef OXYGINE_SDL
#ifdef __MINGW32__
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	run();
	return 0;
}
#else
#include "SDL_main.h"
extern "C" {
	int main(int argc, char* argv[]) {
		run();
		return 0;
	}
};
#endif
#endif
