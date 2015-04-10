/**
 * @file MD3.c
 * @brief Mode 3 code - Animal Game
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 * @author Poornima Kaniarasu (pkaniara)
 */

#include "Globals.h"
#include "Modes.h"
#include "audio.h"
#include "glyph.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "MD3.h"

#define MAX_INDEX 11
#define MODE_FILESET "MD3_"

static char submode = SUBMODE_NULL;
static char next_state = STATE_NULL;
int incorrect_tries = 0;
static char cell = 0;
static char cell_pattern = 0;
static char cell_control = 0;
static bool scrolled = false;
/* user_glyph - inputted glyph from user
 * curr_glyph - glyph corresponding to the next letter in the animal
 * user_word - stores the word as it is being inputted
 * curr_word - stores the entire word
 */ 
static glyph_t* user_glyph = NULL;
static glyph_t* curr_glyph = NULL;
static word_node_t* user_word = NULL;
static word_node_t* curr_word = NULL;
static script_t* this_script = &script_english;


char *animal_list[MAX_INDEX] = {"bee", "camel", "cat", "cow", "dog", "horse",
    "hyena", "pig", "rooster", "sheep", "zebra"};

int animal_used_list[MAX_INDEX] = {0,0,0,0,0,0,0,0,0,0,0};

char* animal_sounds[MAX_INDEX] = {"BEE","CAM","CAT","COW","DOG","HOR",
    "HYE","PIG","ROO","SHE","ZEB"};

/*
* @brief Plays animal sounds
* @param char* animal - Animal name
* @param bool name - true if name requeste, false if sound requested
* @return void
*/
void play_animal(char* animal, bool name) {
  char filename[5];
  for (int i = 0; i < MAX_INDEX; i++) {
    if (strcmp(animal, animal_list[i]) == 0) {
      // Match found
      if (name)
        sprintf(filename, "N%s", animal_sounds[i]);
      else
        sprintf(filename, "S%s", animal_sounds[i]);
      play_mp3(MODE_FILESET, filename);
    }
  }
}


/**
 * @brief Based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - MAX_INDEX
 *        which corresponds to 1 of MAX_INDEX different animals. Check which
 *        animals have been played already to be sure to play all of
 *        the different animals before repeating the list.
 * @return int - number between 1 - MAX_INDEX corresponding to the animal file to play
 */
int choose_animal()
{
  int num = TCNT1;
  int i;

  num *= PRIME;
  num = (abs(num) % MAX_INDEX);

  char buf[10];
  sprintf(buf, "num=%i\r\n", num);
  PRINTF(buf);

  while(animals_used_list[num])
  {
    num = TCNT1;
    num *= PRIME;
    num = (abs(num) % MAX_INDEX);
  }

  animals_used_list[num] = 1;

  for(i = 0; i < MAX_INDEX; i ++)
  {
    sprintf(buf, "arr=%i, ",animals_used_list[i] );
    PRINTF(buf);
  }

  TX_NEWLINE_PC;
  sprintf(buf, "cnt=%i", animals_used+1);
  PRINTF(buf);
  TX_NEWLINE_PC;

  // increment animals_used until we've used all MAX_INDEX animals then reset everything
  animals_used++;
  if (animals_used == MAX_INDEX)
  {
    animals_used = 0;
    for (i = 0; i < MAX_INDEX; i++)
      animals_used_list[i] = 0;
  }

  return num;
}

void md3_reset(void)
{
  set_mode_globals(&script_english, "ENG_", MODE_FILESET);
  next_state = STATE_MENU;
  submode = SUBMODE_NULL;
  incorrect_tries = 0;
  user_glyph = NULL;
  curr_glyph = NULL;
  user_word = free_word(user_word);
  curr_word = free_word(curr_word);
  cell = 0;
  cell_pattern = 0;
  cell_control = 0;
  scrolled = false;
  PRINTF("[MD3] Mode reset");
  play_mp3(MODE_FILESET, "INT");
  play_mp3(MODE_FILESET, "MSEL");
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 * @TODO record MD3INT.MP3
 */
void md3_main(void)
{
  switch(next_state)
  {
    case STATE_MENU:
	  if (io_user_abort == true) {
		  PRINTF("[MD3] Quitting to main menu\n\r");
		  quit_mode();
		  io_init();
	  }
	  cell = get_cell();
	  if (cell == NO_DOTS) {
		  break;	
	  }
	  cell_pattern = GET_CELL_PATTERN(cell);
	  cell_control = GET_CELL_CONTROL(cell);
	  switch (cell_control) {
		  /*If user input is A, enter SUBMODE_PLAY; if input is B, enter SUBMODE_LEARN; 
			else go back to menu*/
		  case WITH_ENTER:
		    user_glyph = search_script(this_script, cell_pattern);
			if (cell_pattern == ENG_A) {
			    play_glyph(user_glyph);
				submode = SUBMODE_PLAY;
				next_state = STATE_GENQUES;
			} else if (cell_pattern == ENG_B) {
				play_glyph(user_glyph);
				submode = SUBMODE_LEARN;
				next_state = STATE_GENQUES;
			} else {
				play_mp3(lang_fileset, MP3_INVALID_PATTERN);
				next_state = STATE_MENU;
			}
		  break;
		  case WITH_LEFT:case WITH_RIGHT:case WITH_CANCEL:
		  break;
	  }
	break;
	  
	case STATE_GENQUES:
	  length_entered_word = 0;
	  current_word_index = 0;
	  animal = animal_list[choose_animal()];
	  user_glyph = NULL;
	  curr_glyph = NULL;
      user_word = free_word(user_word);
	  curr_word = free_word(curr_word);
	  next_state = STATE_PROMPT;
	  break;
	  
    case STATE_PROMPT:
	  switch(submode)
	  {
        case SUBMODE_PLAY:
		  play_mp3(MODE_FILESET, "PLSA");
		  play_animal(animal, false);
		  break;
		
		case SUBMODE_LEARN:
	      play_mp3(MODE_FILESET, "PLSB");
		  play_animal(animal, true);
		  break;
	    
		default:
		  break;  
	  }
	next_state = STATE_INPUT;
	break;
	
	case STATE_INPUT:
	if (io_user_abort == true) {
		PRINTF("[MD3] User aborted input\n\r");
		next_state = STATE_REPROMPT;
		io_init();
	}
	cell = get_cell();
	if (cell == NO_DOTS) {
		break;
	}
	cell_pattern = GET_CELL_PATTERN(cell);
	cell_control = GET_CELL_CONTROL(cell);
	switch (cell_control) {
		case WITH_ENTER:
		user_glyph = search_script(this_script, cell_pattern);
		next_state = STATE_CHECK;
		PRINTF("[MD3] Checking answer\n\r");
		break;
		case WITH_LEFT:
		next_state = STATE_PROMPT;
		break;
		case WITH_RIGHT:
		next_state = STATE_REPROMPT;
		break;
		case WITH_CANCEL:
		break;
	}
	break;

	case STATE_CHECK:
		curr_glyph = search_script(this_script, get_bits_from_letter(animal[length_entered_word]));
		if (glyph_equals(curr_glyph, user_glyph)) {
			play_glyph(curr_glyph);
			incorrect_tries = 0;
			length_entered_word++;
			user_word = add_glyph_to_word(user_word, user_glyph);
			if(length_entered_word != strlen(animal)) {
			  play_mp3(lang_fileset, "GOOD");
			  next_state = STATE_INPUT;
			} else { 
			  play_mp3(lang_fileset,  "GOOD");
			  play_mp3(lang_fileset, "NCWK");
			  switch (submode){
				  case SUBMODE_LEARN:
				  play_animal(animal, true);
				  play_mp3(MODE_FILESET, "SAYS");
				  play_animal(animal, false);
				  break;
				  
				  case SUBMODE_PLAY:
				  play_word(user_word);
				  play_animal(animal, true);
			  }
			  next_state = STATE_GENQUES;
			}
		} else {
			play_glyph(user_glyph);
			incorrect_tries++;
			PRINTF("[MD3] User answered incorrectly\n\r");
			play_mp3(lang_fileset, "NO");
			play_mp3(lang_fileset, MP3_TRY_AGAIN);
			play_word(user_word);
			if (incorrect_tries == MAX_INCORRECT_TRIES_1) {
				play_mp3(MODE_FILESET, "PLWR");
				play_animal(animal, true);
				curr_word = word_to_glyph_word(this_script, animal);
				play_word(curr_word);
			} else if (incorrect_tries >= 6) {
				play_glyph(curr_glyph);
				play_mp3(MODE_FILESET, "PRSS");
				play_dot_sequence(curr_glyph);
			}
			next_state = STATE_INPUT;
		}
		break;
	
	case STATE_REPROMPT:
	switch(create_dialog("SKIP",
	ENTER_CANCEL | LEFT_RIGHT)) {
		case NO_DOTS:
		break;

		case CANCEL:
		PRINTF("[MD3] Reissuing prompt");
		next_state = STATE_PROMPT;
		scrolled = false;
		break;

		case ENTER:
		PRINTF("[MD3] Skipping animal");
		if (scrolled)
		next_state = STATE_PROMPT;
		else
		next_state = STATE_GENQUES;
		scrolled = false;
		break;

		case RIGHT: case LEFT:
			PRINTF("[MD3] Next animal");
			length_entered_word = 0;
			current_word_index = 0;
			animal = animal_list[choose_animal()];
			user_word = NULL;
			switch(submode)
			{
				case SUBMODE_PLAY:
				play_animal(animal, false);
				break;
				
				case SUBMODE_LEARN:
				play_animal(animal, true);
				break;
				
				default:
				break;
			}
			scrolled = true;
		break;

		default:
		break;
	}
	break;

	default:
	break;
  }
}

