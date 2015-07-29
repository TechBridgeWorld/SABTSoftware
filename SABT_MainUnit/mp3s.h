// all prefixes used
#define MP3_SYSTEM     "s_"
#define MP3_DIRECTIONS "d_"
#define MP3_FEEDBACK   "f_"
#define MP3_ENGLISH    "e_"
#define MP3_HINDI      "h_"
#define MP3_KANNADA	   "k_"


// these exist in both e_ (used by English & Hindi modes) and k_ (used by Kannada)
#define MP3_DOT_1 "dot_1"
#define MP3_DOT_2 "dot_2"
#define MP3_DOT_3 "dot_3"
#define MP3_DOT_4 "dot_4"
#define MP3_DOT_5 "dot_5"
#define MP3_DOT_6 "dot_6"
#define MP3_DOT_C "dot_c"
#define MP3_DOT_E "dot_e"



////////////// SYSTEM-WIDE ////////////////

#define MP3_WELC  "welcom"  // Welcome to the stand-alone braille tutor.
#define MP3_MM    "mm"      // Main menu.
#define MP3_MENU  "menu"    // You are in the main menu. Use the left and right buttons to choose a mode. Then press enter to select the mode.

#define MP3_TADA  "tada"    // tada sound
#define MP3_VOL   "volpop"  // pop sound
#define MP3_WOMP  "womp"    // sad trombone sound

#define MP3_BACQ  "retqes"	// Returning to the question now. (Susan's voice.) // not used?

#define MP3_S025  "s025" 	// 250 milliseconds silence
#define MP3_S050  "s050"	// 500 milliseconds silence
#define MP3_S075  "s075" 	// 750 milliseconds silence
#define MP3_S100  "s100" 	// 1 second silence


//////// DIRECTIONS ////////
	

// mode instructions
#define MP3_INSTRUCTIONS_LETTER	"instlt"	// used in modes 2, 7, 8 (learnletter)
// To submit your answer, please press enter. To hear the letter again, press left. To change the letter, press right. To clear a cell, press cancel.

#define MP3_INSTRUCTIONS_MATH	"instma"	// used in modes 9, 13
// To submit your answer, press enter. To move to the next or previous cell, use left or right. To clear a cell, press cancel. For help, press cancel two times.

#define MP3_INSTRUCTIONS_WORD	"instwd" // used in modes 14, 15
// Spell the word, pressing enter after each letter. Press left to repeat the word. Press right to skip to the next word. Press cancel to erase your entry.

#define MP3_INSTRUCTIONS_KANNADA	"instKA" // used in mode 12
// presumably the same as MP3_INSTRUCTIONS_LETTER, but in Kannada


// what the command buttons do
#define MP3_LETTER_COMMANDS	"ctrllt"	// Mode 2:  To move to the next letter, press right. To move to the previous letter, press left. To select a letter, press enter.
#define MP3_WORD_COMMANDS	"ctrlwd"	// To skip the question, press enter. To try again, press cancel. To hear the question again, press left. To hear the answer, press right.
#define MP3_HINDI_COMMANDS  "ctrlhi"	// ???


#define MP3_HELP_MENU_LETTER	"helplt"
// To skip this letter, press enter. To try again, press cancel. To change the letter, use the left and right buttons and then press enter.
// NOT CURRENTLY USED?

#define MP3_HELP_MENU_KANNADA	"helplK"
// Kannada: To skip this letter, press enter. To try again, press cancel. To change the letter, use the left and right buttons and then press enter.

#define MP3_HELP_MENU_NUMBER	"helpnm"
// To skip this number, press enter. To try again, press cancel. To change the number, use the left and right buttons and then press enter.

#define MP3_HELP_MENU_OBJECT	"helpob"
// To skip this object, press enter. To continue, press cancel.	

#define MP3_HELP_MENU_ANIMAL 	"helpan"
// To skip this animal, press enter. To continue playing, press cancel.

#define MP3_HELP_MENU_13		"help13"
// You are in the help menu. To hear the question again, press left. To hear the answer and skip the question, perss right. To hear the answer you have entered so far, press enter. To exit the help menu, press cancel. To exit the mode, long press cancel.


// Does this one exist? // To skip this letter, press enter. To continue playing, press cancel.


// instructions for gameplay
#define MP3_FIND_DOT      	"finddt"    // Find dot:
#define MP3_PRESS_DOTS    	"prssdt"    // Press dots:
#define MP3_PRESS_DOTS_K   	"prssdK"    // Press dots:  [Kannada]
#define MP3_PLEASE_PRESS  	"press"     // Please press:
#define MP3_PLEASE_WRITE	"write"		// Please write:
#define MP3_GUESS_A_LETTER 	"guessl"    // Guess a letter
#define MP3_SPELL_WORD    	"spellw"    // Please spell the word:
#define MP3_NEW_WORD		"newwrd"    // New word:
#define MP3_ENTER_NEXT_NUM	"nexnum"    // Enter the next number

#define MP3_WRITE_NUMBER_	"numequ"    // Please write the number that is equal to:
#define MP3_ENTER_NEW_WORD  "entrnw"      // Please enter a new word.
#define MP3_WHAT_IS         "whatis"    // What is:   // THIS DOESN'T SEEM TO EXIST?


// level/difficulty selection
#define MP3_CHOOSE_NUM_OF_HINTS "hints" // Select the number of hints you want.
#define MP3_CHOOSE_LEVELS_3 "chlev3"    // Choose your difficulty level. Press 1 for easy, 2 for medium, or 3 for hard.
#define MP3_CHOOSE_LEVELS_2 "chlev2"    // Choose your difficulty level. Press 1 for easy, or press 2 for hard.
#define MP3_EASY_MODE		"mdeasy"    // You are in easy mode.
#define MP3_MEDIUM_MODE 	"mdmed"     // You are in medium mode. (THIS DOES NOT EXIST.)
#define MP3_HARD_MODE 		"mdhard"    // You are in hard mode.
#define MP3_UNDER_DEVEL		"devel"     // Submode under development. Please press cancel to exit to submode menu.


// players, for player modes
#define MP3_PLAYER_1 		"playr1"    // Player 1:
#define MP3_PLAYER_2		"playr2"    // Player 2:

// Next/previous cell/letter/word
#define MP3_FIRST_CELL		"frstcl"    // First cell:
#define MP3_PREV_CELL		"prevcl"    // Previous cell.
#define MP3_NEXT_CELL		"nextcl"    // Next cell:
#define MP3_LAST_CELL		"lastcl"    // Last cell:
#define MP3_NEXT_LETTER		"nextlt"    // Next letter:
#define MP3_NEXT_PATTERN 	"nextpt"    // Next pattern.

#define MP3_FIRST_CELL_K	"frstck"    // First cell: [Kannada]
#define MP3_PREV_CELL_K		"prevck"    // Previous cell. [Kannada]
#define MP3_NEXT_CELL_K		"nextck"    // Next cell: [Kannada]
#define MP3_LAST_CELL_K		"lastck"    // Last cell: [Kannada]
#define MP3_NEXT_LETTER_K	"nextlt"    // Next letter:
#define MP3_NEXT_PATTERN_K 	"nextpt"    // Next pattern.

// math instructions
#define MP3_PLEASE_ENTER_NUM_BETWEEN 	"BETW"    // Please enter a number between:
#define MP3_ENTER_NUM_BETWEEN	"entnum"    // Enter the number between:
#define MP3_ENTER_NUMS_BETWEEN	"entnms"    // Enter the numbers between:
#define MP3_ENTER_EVENS_BTWN	"evens"    // Enter the even numbers between:
#define MP3_HOW_MANY_HUNDREDS  	"divhun"    // How many hundreds are in the number you just entered?
#define MP3_ROUND_TO_HUNDREDS 	"roundh"    // Round the following number to the nearest hundred.
#define MP3_ENTER_ODDS_BTWN  	"odds"    // Enter the odd numbers between
#define MP3_HOW_MANY_TENS		"divten"    // How many tens are in the number you just entered?
#define MP3_ROUND_TO_TENS		"roundt"    // Round the following number to the nearest ten.

#define MP3_PLUS 			"plus"		// plus
#define MP3_MINUS			"minus"     // minus
#define MP3_TIMES 			"times"    	// into

// misc
#define MP3_AND 			"and"     	// and
#define MP3_SAYS 			"says"    	// says
#define MP3_PLEASE_WAIT		"wait"    	// Please wait.



//////// FEEDBACK ////////

// Feedback on what you pressed.
#define MP3_YOU_PRESSED_DOT "uprsdt"  	// You pressed dot:
#define MP3_YOU_ANSWERED 	"uansrd"    // You answered:
#define MP3_SPELLING_SO_FAR "ssofar"    // Your spelling so far is:

// Correct
#define MP3_YES  			"yes"     	// Yes.
#define MP3_GOOD    		"good"    	// Good!
#define MP3_GOOD_NEXT		"goodnx"	// Good! Next:  // @todo listen to this
#define MP3_CORRECT 		"corrct"    // Correct answer!
#define MP3_CORRECT_K 		"corrcK"    // Correct answer! [Kannada]
#define MP3_NICE_WORK 		"nicewk"    // Nice work

// Incorrect/invalid
#define MP3_NO   			"no"      	// No.
#define MP3_TRY_AGAIN 		"tryagn"    // Try again.
#define MP3_TRY_AGAIN_K		"tryagk"    // Try again. [Kannada]
#define MP3_INCORRECT 		"incorrc"   // Incorrect answer.
#define MP3_INCORRECT_K		"incorrK"   // Incorrect answer -- KANNADA
#define MP3_WORD_YOU_MISSED "wrdmis"    // The word you missed was:
#define MP3_YOU_MISSED 		"umissd"	// You missed:

#define MP3_INVALID_PATTERN "invpat"    // Invalid pattern. Please try again.
#define MP3_INVALID_PATTERN_K "invpaK"  // Invalid pattern. Please try again.  [Kannada]
#define MP3_NO_NUMBER_13	"nonum"    	// @todo check this one
#define MP3_NO_NUMBER 		"nonumr"    // No number is entered. Please try again.
#define MP3_NO_NUMBER_K 	"nonumK"    // No number is entered. Please try again.  [Kannada]

#define MP3_WOSM 			 "nohass"      // You entered and answer without a number symbol.
#define MP3_NO_NUMBER_SIGN 	 "nohash"    // You entered an answer without a number symbol. Please enter your answer including the number symbol, which is dots 3, 4, 5 and 6.
#define MP3_NO_NUMBER_SIGN_K "nohasK"   // You entered an answer without a number symbol. Please enter your answer including the number symbol, which is dots 3, 4, 5 and 6.   [Kannada]

#define MP3_TOO_LONG      	"wdlong"    // This word is too long. Please start over and choose a word that is fewer than 10 letters long.
#define MP3_WORD_NOT_FOUND 	"wdnotf"    // Word not found in dictionary. Please try again.

// Housekeeping
#define MP3_HELP_MENU		"helpm"   	// Help menu.
#define MP3_EXIT_HELP_MENU	"helpx"   	// Exit help menu.
#define MP3_RET_TO_QUESTION "retques"   // Returning to the question now.


// Update on your word and stats/mistakes so far
#define MP3_IS_SPELLED  		"isspld"	// is spelled
#define MP3_YOUR_WORD_IS 		"yorwrd"    // Your word is:
#define MP3_YOUR_WORD_IS_NOW 	"wrdnow"    // Your word is now:
#define MP3_THE_ANSWER_IS 		"answer"    // The answer is:
#define MP3_YOU_HAVE_SPELLED 	"uhvspll"   // You have spelled:
#define MP3_WORD                "word"      // word.
#define MP3_WORDS               "words"     // words.
#define MP3_WORD_AND_HAVE_MADE 	"wrdand"    // word and have made
#define MP3_WORDS_AND_HAVE_MADE "wrdsan"    // words and have made
#define MP3_AND_YOU_HAVE_MADE 	"uhvmad"    // and you have made
#define MP3_YOU_HAVE 			"youhav"    // You have:
#define MP3_MISTAKE             "mistak"    // mistake.
#define MP3_MISTAKES            "mistks"    // mistakes.
#define MP3_MISTAKE_REMAINING 	"mislft"    // mistake remaining
#define MP3_MISTAKES_REMAINING 	"misslf"    // mistakes remaining
#define MP3_YOU_HAVE_MADE_THE_SAME_MISTAKE "sammst"    // You have made the same mistake.


#define MP3_YOUR_SCORE_IS			"SCOR"    // Your current score is:
#define MP3_NUM_MISSED 				"MISS"    // You missed:
#define MP3_MULT_MISTAKES 			"MLFT"    // mistakes remaining.
#define MP3_ONE_MISTAKE				"MONE"    // mistake remaining.
#define MP3_NO_ANSWER_SO_FAR		"NOCO"    // No correct answer entered so far.
#define MP3_NEXT 					"NXTN"    // Enter the next number.


// Game over
#define MP3_YOU_HAVE_GUESSED_THE_WORD	"youwin"    // You have guessed the word.
#define MP3_7_MISTAKES_YOU_MISSED	 	"lose7m"    // You have made seven mistakes. The word you missed was:
#define MP3_GAME_OVER_DRAW 				"windrw"    // Game over. The game was a draw.
#define MP3_GAME_OVER_WINNER_1 			"winpl1"    // Game over. The winner is player 1.
#define MP3_GAME_OVER_WINNER_2 			"winpl2"    // Game over. The winner is player 2.

// Device-passing for two-player modes
#define MP3_VALID_WORD_PASS_DEVICE 		"passvl"    // Valid word. Please hand device to player 2 and hand device to player 2 and press enter when ready to play.
#define MP3_PASS_DEVICE_PRESS_ENTER 	"passbt"    // Please hand device to player 2 and press enter when ready to play.



//////// ENGLISH ///////
#define MP3_BLANK		"blank"
#define MP3_CAPITAL		"capitl"

//////// KANNADA ///////
#define MP3_BLANK_K		"blank"


// mode specific


#define MP3_PLSA PLSA
	// mode 3:	Please write the name of the animal that makes the sound:
	// mode 11:	Please write the name of the object that makes the sound:

#define MP3_PLSB PLSB
	// mode 3:  Please write the spelling of this animal.
	// mode 11: Please write the spelling of this object:






#define MP3_ZERO       	"#0"
#define MP3_ONE        	"#1"
#define MP3_TEN       	"#10"
#define MP3_ELEVEN    	"#11"
#define MP3_TWELVE     	"#12"
#define MP3_THIRTEEN  	"#13"
#define MP3_FOURTEEN    "#14"
#define MP3_FIFTEEN     "#15"
#define MP3_SIXTEEN     "#16"
#define MP3_SEVENTEEN   "#17"
#define MP3_EIGHTEEN    "#18"
#define MP3_NINETEEN    "#19"
#define MP3_TWO         "#2"
#define MP3_TWENTY      "#20"
#define MP3_THREE 		"#3"
#define MP3_THIRTY		"#30"
#define MP3_FOUR   		"#4"
#define MP3_FORTY 		"#40"
#define MP3_FIVE 		"#5"
#define MP3_FIFTY 		"#50"
#define MP3_SIX 		"#6"
#define MP3_SIXTY 		"#60"
#define MP3_SEVEN 		"#7"
#define MP3_SEVENTY 	"#70"
#define MP3_EIGHT 		"#8"
#define MP3_EIGHTY 		"#80"
#define MP3_NINE 		"#9"
#define MP3_NINETY 		"#90"
#define MP3_HUNDRED 	"hundr"
#define MP3_NEGATIVE 	"negtv"
#define MP3_NUM_SIGN 	"numsn"
#define MP3_THOUSAND 	"thous"




////////// mode files - no suffix

/*
#define  MD1 		// Learn dots.
#define  MD2 		// Learn letters.
#define  MD3 		// Animal sounds.
#define  MD4 		// Hangman.
#define  MD5 		// Two-player hangman.
#define  MD6 		// Free play.
#define  MD7 		// Hindi ??
#define  MD8 		// Numbers?
#define  MD9 		// Maths practice
#define  MD10 		// Contractions
#define  MD11 		// Everyday noises game.
#define  MD12 		// [Kannada mode]
#define  MD13 		// Number game?
#define  MD14 		// Spelling practice
#define  MD15 		// Spelling bee
*/

////////// mode-specific ///////////


#define MP3_WELCOME 		"welc"
	// Mode 1:  Welcome to the number game.
	// Mode 2:  Welcome to letter practice.
	// Mode 3:  Welcome to the animal game.
	// Mode 4:  Welcome to hangman.
	// Mode 5:  Welcome to two-player hangman. Player 1, please enter a word for the other player to guess. Press enter between each letter, and twice at the end of the word. Do not let the other player hear you do this.
	// Mode 6:  Welcome to free play. Press the pattern for any letter, and then press enter.
	// Mode 7:  Welcome to Hindi letter practice. To learn letters, press dot 1. To practice letters, press dot 2.
	// Mode 8:  Welcome to number practice. To learn numbers, press dot 1. To practice numbers, press dot 2.
	// Mode 9: 
	// Mode 10: Welcome to contraction practice.   (WEL1); Welcome. Please select a submode. (INT)
	// Mode 11: Welcome to the everyday noises game.
	// Mode 12:
	// Mode 13:
	// Mode 14: Welcome to the spelling practice.
	// Mode 15: Welcome to the two-player spelling bee. Each of you will be asked to spell:




#define MP3_SUBMODE      "subm"
  	// Mode 2: To learn letters press dot 1, to practice letters press dot 2.
	// Mode 3: To learn animal sounds, press dot 1. To play the game, press dot 2.
	// Mode 4: Fill in the blanks without more than eight mistakes. Please press dot 1 for one hint, press dot 2 for two hints, press dot 3 for three hints. Press enter for no hints.
	// Mode 9: For addition, press dot 1. For subtraction, dot 2. For multiplication, dot 3.   MENU
	// Mode 10: Use left and right buttons to select a submode. Press enter to choose a submode.   MSEL
	// Mode 11: To learn everyday noises, press dot 1. To play the game, press dot 2.
	// Mode 12: [In Kannada] -- MENU





/*

#define MP3_N/S BEE N/S BEE    // bee
#define MP3_N/S CAM N/S CAM    // camel
#define MP3_N/S CAT N/S CAT    // cat
#define MP3_N/S COW N/S COW    // cow
#define MP3_N/S DOG N/S DOG    // dog
#define MP3_N/S HOR N/S HOR    // horse
#define MP3_N/S HYE N/S HYE    // hyena
#define MP3_N/S PIG N/S PIG    // pig
#define MP3_N/S ROO N/S ROO    // rooster
#define MP3_N/S SHE N/S SHE    // sheep
#define MP3_N/S ZEB N/S ZEB    // zebra



////// mode 5 /////////


#define MP3_AMSK AMSK    // And you have made
#define MP3_LVLS LVLS    // Select the number of hints you want.
#define MP3_MPRE MPRE    // You have
#define MP3_MSTK MSTK    // Mistakes.
#define MP3_NWOR NWOR    // New word
#define MP3_SOFA SOFA    // Your word is now
#define MP3_TAIS TAIS    // The answer is
#define MP3_YOLO YOLO    // The word you missed was:
#define MP3_YOWI YOWI    // You have guessed the word.



////// mode 10 /////////

*/
#define MP3_CELL_1 "CL1"    // Cell 1
#define MP3_CELL_2 "CL2"    // Cell 2
#define MP3_DICT "DICT"    // Dictation mode.
#define MP3_PABR "PABR"    // Practice abbreviations.
#define MP3_PCON "PC"    // Practice contractions



/*

////// mode 11 /////////


#define MP3_LIKE LIKE    // sounds like
#define MP3_N/S AER N/S AER    // Aeroplane
#define MP3_N/S AUT N/S AUT    // Auto
#define MP3_N/S BEL N/S BEL    // Bell
#define MP3_N/S CLO N/S CLO    // Clock
#define MP3_N/S DOO N/S DOO    // Door
#define MP3_N/S HOR N/S HOR    // Horn
#define MP3_N/S PHO N/S PHO    // Phone
#define MP3_N/S RAI N/S RAI    // Rain
#define MP3_N/S SIR N/S SIR    // Siren
#define MP3_N/S TRA N/S TRA    // Train
#define MP3_N/S TRU N/S TRU    // Truck

*/


///////// letter files -- never called directly /////////


/*
#define  "ENG_A"
#define  "ENG_B"
#define  "ENG_C"
#define  "ENG_D"
#define  "ENG_E"
#define  "ENG_F"
#define  "ENG_G"
#define  "ENG_H"
#define  "ENG_I"
#define  "ENG_J"
#define  "ENG_K"
#define  "ENG_L"
#define  "ENG_M"
#define  "ENG_N"
#define  "ENG_O"
#define  "ENG_P"
#define  "ENG_Q"
#define  "ENG_R"
#define  "ENG_S"
#define  "ENG_T"
#define  "ENG_U"
#define  "ENG_V"
#define  "ENG_W"
#define  "ENG_X"
#define  "ENG_Y"
#define  "ENG_Z"



#define  HIN_A
#define  HIN_AA
#define  HIN_AHA
#define  HIN_AI
#define  HIN_AM
#define  HIN_AU
#define  HIN_BA
#define  HIN_BHA
#define  HIN_CHA
#define  HIN_CHHA
#define  HIN_DA
#define  HIN_DDA
#define  HIN_DDHA
#define  HIN_DHA
#define  HIN_EE
#define  HIN_GA
#define  HIN_GHA
#define  HIN_GNA
#define  HIN_HA
#define  HIN_I
#define  HIN_II
#define  HIN_JA
#define  HIN_JHA
#define  HIN_JNA
#define  HIN_KA
#define  HIN_KHA
#define  HIN_KSHA
#define  HIN_LA
#define  HIN_MA
#define  HIN_NA
#define  HIN_NNA
#define  HIN_NYA
#define  HIN_NYAA
#define  HIN_OO
#define  HIN_PA
#define  HIN_PHA
#define  HIN_RA
#define  HIN_RU
#define  HIN_SA
#define  HIN_SHA
#define  HIN_SHHA
#define  HIN_SHRA
#define  HIN_TA
#define  HIN_THA
#define  HIN_TRA
#define  HIN_TTA
#define  HIN_TTHA
#define  HIN_U
#define  HIN_UU
#define  HIN_VA
#define  HIN_YA

#define  KAN_A
#define  KAN_AA
#define  KAN_AHA
#define  KAN_AI
#define  KAN_AM
#define  KAN_AU
#define  KAN_BA
#define  KAN_BHA
#define  KAN_CHA
#define  KAN_CHHA
#define  KAN_DA
#define  KAN_DDA
#define  KAN_DDHA
#define  KAN_DHA
#define  KAN_DLA
#define  KAN_E
#define  KAN_EE
#define  KAN_GA
#define  KAN_GHA
#define  KAN_GNA
#define  KAN_HA
#define  KAN_I
#define  KAN_II
#define  KAN_JA
#define  KAN_JHA
#define  KAN_KA
#define  KAN_KHA
#define  KAN_LA
#define  KAN_MA
#define  KAN_NA
#define  KAN_NLET
#define  KAN_NNA
#define  KAN_NYA
#define  KAN_NYAA
#define  KAN_O
#define  KAN_OO
#define  KAN_PA
#define  KAN_PHA
#define  KAN_RA
#define  KAN_RU
#define  KAN_SA
#define  KAN_SHA
#define  KAN_SHHA
#define  KAN_TA
#define  KAN_THA
#define  KAN_TTA
#define  KAN_TTHA
#define  KAN_U
#define  KAN_UU
#define  KAN_VA
#define  KAN_YA
*/


///////// vocab files  -- never called directly ///////////

/*#define  V_Arctic
#define  V_absorb
#define  V_adapt
#define  V_air
#define  V_aquar0
#define  V_aquat0
#define  V_atmos0
#define  V_bite
#define  V_blade
#define  V_bloom
#define  V_branch
#define  V_brook
#define  V_bud
#define  V_bug
#define  V_canopy
#define  V_canyon
#define  V_cave
#define  V_cellu0
#define  V_cliff
#define  V_cloud
#define  V_coast
#define  V_conde0
#define  V_conse0
#define  V_conse1
#define  V_creat0
#define  V_cycle
#define  V_damp
#define  V_day
#define  V_decay
#define  V_den
#define  V_descr0
#define  V_desert
#define  V_dew
#define  V_dig
#define  V_dirt
#define  V_dispo0
#define  V_drip
#define  V_drop
#define  V_droug0
#define  V_dry
#define  V_dump
#define  V_earth
#define  V_earth0
#define  V_east
#define  V_ecosy0
#define  V_edge
#define  V_energy
#define  V_envir0
#define  V_erosi0
#define  V_erupt
#define  V_evapo0
#define  V_extin0
#define  V_fall
#define  V_fast
#define  V_fern
#define  V_ferti0
#define  V_fit
#define  V_float
#define  V_flood
#define  V_flower
#define  V_fly
#define  V_fog
#define  V_force
#define  V_forest
#define  V_fossil
#define  V_freez0
#define  V_fresh
#define  V_frict0
#define  V_funct0
#define  V_fungus
#define  V_gas
#define  V_gorge
#define  V_grass
#define  V_gravi0
#define  V_ground
#define  V_growth
#define  V_habit0
#define  V_hemis0
#define  V_herbi0
#define  V_hiber0
#define  V_hill
#define  V_hive
#define  V_hole
#define  V_humid
#define  V_insect
#define  V_inves0
#define  V_joint
#define  V_lake
#define  V_larva
#define  V_lava
#define  V_leaf
#define  V_life
#define  V_light
#define  V_liquid
#define  V_living
#define  V_log
#define  V_low
#define  V_magnet
#define  V_mammal
#define  V_mangr0
#define  V_marsh
#define  V_mass
#define  V_matter
#define  V_melt
#define  V_melti0
#define  V_mist
#define  V_moist
#define  V_moist0
#define  V_mold
#define  V_molten
#define  V_monso0
#define  V_moon
#define  V_morni0
#define  V_motion
#define  V_mount0
#define  V_mudsl0
#define  V_nature
#define  V_nest
#define  V_night
#define  V_nonli0
#define  V_north
#define  V_ocean
#define  V_offsp0
#define  V_oil
#define  V_omniv0
#define  V_orbit
#define  V_organ0
#define  V_path
#define  V_patte0
#define  V_planet
#define  V_plant
#define  V_plates
#define  V_pollen
#define  V_pollu0
#define  V_pollu1
#define  V_pond
#define  V_preci0
#define  V_prote0
#define  V_rain
#define  V_rainf0
#define  V_raw
#define  V_rays
#define  V_record
#define  V_recyc0
#define  V_renew
#define  V_renew0
#define  V_repel
#define  V_resem0
#define  V_resor0
#define  V_rest
#define  V_revol0
#define  V_ridge
#define  V_ripe
#define  V_river
#define  V_rock
#define  V_rocks0
#define  V_root
#define  V_rot
#define  V_rotat0
#define  V_rotten
#define  V_salt
#define  V_sap
#define  V_sea
#define  V_season
#define  V_sedim0
#define  V_seed
#define  V_selec0
#define  V_sleep
#define  V_slow
#define  V_soil
#define  V_solid
#define  V_sound
#define  V_south
#define  V_spring
#define  V_sprout
#define  V_star
#define  V_steam
#define  V_stem
#define  V_sting
#define  V_stone
#define  V_strai0
#define  V_stream
#define  V_struc0
#define  V_summer
#define  V_sun
#define  V_sunri0
#define  V_suppo0
#define  V_survi0
#define  V_system
#define  V_tempe0
#define  V_textu0
#define  V_tide
#define  V_top
#define  V_tree
#define  V_tsuna0
#define  V_tundra
#define  V_under
#define  V_valley
#define  V_varia0
#define  V_vibra0
#define  V_wash
#define  V_waste
#define  V_water
#define  V_water0
#define  V_weath0
#define  V_web
#define  V_weed
#define  V_west
#define  V_wet
#define  V_wind
#define  V_winter
#define  V_wood
#define  V_zigzag */

//////// Contraction files -- not currently used ////////
/*
#define MP3_CON0_W1  CON0_W1     // a
#define MP3_CON0_W2  CON0_W2     // but
#define MP3_CON0_W3  CON0_W3     // can
#define MP3_CON0_W4  CON0_W4     // do
#define MP3_CON0_W5  CON0_W5     // every
#define MP3_CON0_W6  CON0_W6     // from
#define MP3_CON0_W7  CON0_W7     // go
#define MP3_CON0_W8  CON0_W8     // have
#define MP3_CON0_W9  CON0_W9     // I
#define MP3_CON0_W10 CON0_W10    // just
#define MP3_CON0_W11 CON0_W11    // knowledge
#define MP3_CON0_W12 CON0_W12    // like
#define MP3_CON0_W13 CON0_W13    // more
#define MP3_CON0_W14 CON0_W14    // not
#define MP3_CON0_W15 CON0_W15    // oh??
#define MP3_CON0_W16 CON0_W16    // people
#define MP3_CON0_W17 CON0_W17    // quite
#define MP3_CON0_W18 CON0_W18    // rather
#define MP3_CON0_W19 CON0_W19    // so
#define MP3_CON0_W20 CON0_W20    // that
#define MP3_CON0_W21 CON0_W21    // us
#define MP3_CON0_W22 CON0_W22    // very
#define MP3_CON0_W23 CON0_W23    // will
#define MP3_CON0_W24 CON0_W24    // it
#define MP3_CON0_W25 CON0_W25    // you
#define MP3_CON0_W26 CON0_W26    // as
#define MP3_CON0_W27 CON0_W27    // and
#define MP3_CON0_W28 CON0_W28    // far
#define MP3_CON0_W29 CON0_W29    // of
#define MP3_CON0_W30 CON0_W30    // the
#define MP3_CON0_W31 CON0_W31    // thick??
#define MP3_CON0_W32 CON0_W32    // child
#define MP3_CON0_W33 CON0_W33    // shall
#define MP3_CON0_W34 CON0_W34    // this
#define MP3_CON0_W35 CON0_W35    // which
#define MP3_CON0_W36 CON0_W36    // out
#define MP3_CON1_W4  CON1_W4     // day
#define MP3_CON1_W5  CON1_W5     // ever
#define MP3_CON1_W6  CON1_W6     // father
#define MP3_CON1_W8  CON1_W8     // here
#define MP3_CON1_W11 CON1_W11    // know
#define MP3_CON1_W12 CON1_W12    // lord
#define MP3_CON1_W13 CON1_W13    // mother
#define MP3_CON1_W14 CON1_W14    // name
#define MP3_CON1_W15 CON1_W15    // one
#define MP3_CON1_W16 CON1_W16    // part
#define MP3_CON1_W17 CON1_W17    // question
#define MP3_CON1_W18 CON1_W18    // right
#define MP3_CON1_W19 CON1_W19    // some
#define MP3_CON1_W20 CON1_W20    // time
#define MP3_CON1_W21 CON1_W21    // under
#define MP3_CON1_W23 CON1_W23    // work
#define MP3_CON1_W25 CON1_W25    // young
#define MP3_CON1_W30 CON1_W30    // there?
#define MP3_CON1_W32 CON1_W32    // character
#define MP3_CON1_W34 CON1_W34    // through
#define MP3_CON1_W35 CON1_W35    // where?
#define MP3_CON1_W36 CON1_W36    // ought
#define MP3_CON2_W21 CON2_W21    // upon
#define MP3_CON2_W23 CON2_W23    // word
#define MP3_CON2_W30 CON2_W30    // these
#define MP3_CON2_W34 CON2_W34    // those
#define MP3_CON2_W35 CON2_W35    // whose
#define MP3_CON3_W3  CON3_W3     // cannot
#define MP3_CON3_W8  CON3_W8     // hard
#define MP3_CON3_W13 CON3_W13    // many
#define MP3_CON3_W19 CON3_W19    // spirit
#define MP3_CON3_W23 CON3_W23    // world
#define MP3_CON3_W30 CON3_W30    // there
#define MP3_CON4_W4  CON4_W4     // -ount?
#define MP3_CON4_W5  CON4_W5     // -ance?
#define MP3_CON4_W14 CON4_W14    // -tion
#define MP3_CON4_W19 CON4_W19    // -less
#define MP3_CON4_W20 CON4_W20    // -ount-?
#define MP3_CON5_W5  CON5_W5     // -ence?
#define MP3_CON5_W7  CON5_W7     // -on? ong?
#define MP3_CON5_W12 CON5_W12    // -ful
#define MP3_CON5_W14 CON5_W14    // -tion?
#define MP3_CON5_W19 CON5_W19    // -ness
#define MP3_CON5_W20 CON5_W20    // -ment
#define MP3_CON5_W25 CON5_W25    // -ity
#define MP3_CON6_W14 CON6_W14    // -ally
#define MP3_CON6_W25 CON6_W25    // -ation */