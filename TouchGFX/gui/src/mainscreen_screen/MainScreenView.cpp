#include <gui/mainscreen_screen/MainScreenView.hpp>

//#define LOGGING
extern"C" {
	#include "RND_Main.h"
	extern QueueHandle_t 	printQueue;
}

typedef enum
{
	s_IDDLE,
	s_FADEOUT,
	s_FADEOUT_WAIT,
	s_PRINT,
	s_FADEIN,
}	s_state;

MainScreenView::MainScreenView()
{
}

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();
}

void MainScreenView::tearDownScreen()
{
    MainScreenViewBase::tearDownScreen();
}

/* called by tick handling from upper model */
void MainScreenView::printText( void)
{
char *p;
static char txt[64];
static s_state state = s_IDDLE;

	if( printQueue)
	{
		switch( state)
		{
			case s_IDDLE:
				if( xQueueReceive( printQueue, &p, 0) == pdPASS)
				{
					LOG("Received \"%s\" for output to screen\n", p);

					if( !strcmp(p,"start"))
						application().gotoIntroScreenScreenNoTransition();
					else //if( strcmp( txt, p))
					{
						memset( (void *)txt, 0, sizeof(txt));
						strcpy( txt, p);
						state = s_FADEOUT;
					}
				}
				break;

			case s_FADEOUT:
				textArea.startFadeAnimation(0, 3);
				state = s_PRINT;
				break;

			case s_PRINT:
				if( textArea.isFadeAnimationRunning() == false)
				{
					Unicode::fromUTF8((const uint8_t *)txt, textAreaBuffer, TEXTAREA_SIZE);
					textArea.invalidate();
					textArea.startFadeAnimation(255, 6 );
					state = s_FADEIN;
				}
				break;

			case s_FADEIN:
				if( textArea.isFadeAnimationRunning() == false)
					state = s_IDDLE;
				break;

			default:
				break;
		}
	}
}
