// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexSprees nexSpree;

// Customizable killing spree spam
// [c] - current spree count
// [v] - victim name
// [n] - clean victim name
// [t] - total kills
// [m] - double/triple/multikill msgs
// [h] - player name
// [d] - mod name
void CNexSprees::killSpam() {
	char spam[1024];
	char send[1024];
	memset(spam, 0, sizeof(spam));
	memset(send, 0, sizeof(send));

	if (!nexValue[VAR_KILLSPAM])
		return;

	if (!strlen(sNex.ksFormat)) {
		#ifdef NEX_LOGGING
			nexUtils.log("Killspam enabled, but killspam format isn't specified");
		#endif
		return;
	}

	int a, b, len = strlen(sNex.ksFormat);
	for (a = 0, b = 0; a < len; a++) {
		if ((a <= len-2) && (sNex.ksFormat[a] == '[' && sNex.ksFormat[a+2] == ']')) {
			switch (sNex.ksFormat[a+1]) {
				case 'v':
					strncat(spam, va("%s", nex.client[nex.stats.lastVictim].name), sizeof(spam));
					break;
				case 'n':
					strncat(spam, va("%s", nex.client[nex.stats.lastVictim].cleanName), sizeof(spam));
					break;
				case 't':
					strncat(spam, va("%i", nex.stats.killCount), sizeof(spam));
					break;
				case 'c':
					strncat(spam, va("%i", nex.stats.killCountNoDeath), sizeof(spam));
					break;
				case 'h':
					strncat(spam, va("%s", nex.client[nex.cg_clientNum].name), sizeof(spam));
					break;
				case 'd':
					strncat(spam, va("%s", nex.mod->name), sizeof(spam));
					break;
				case 'm': {
					char *multikillmsg;
					int randmsg = rand() % 3;
					if (nex.stats.killSpreeCount == 1) {
						if (!randmsg)
							multikillmsg = "EATEN";
						else if (randmsg == 1)
							multikillmsg = "OWNED";
						else
							multikillmsg = "ADIOS AMIGO";
					}       
					else if (nex.stats.killSpreeCount == 2) {
						if (!randmsg)
							multikillmsg = "DOUBLE KILL";
						else if (randmsg == 1)
							multikillmsg = "PERFECT";
						else
							multikillmsg = "OWNED";
					}
					else if (nex.stats.killSpreeCount == 3) {
						if (!randmsg)
							multikillmsg = "TRIPLE KILL";
						else if (randmsg == 1)
							multikillmsg = "GODLIKE";
						else
							multikillmsg = "LUDICROUS";
					}
					else if (nex.stats.killSpreeCount == 4) {
						if (!randmsg)
							multikillmsg = "DOMINATING";
						else if (randmsg == 1)
							multikillmsg = "ULTRA KILL";
						else
							multikillmsg = "MEGA KILL";
					}
					else if (nex.stats.killSpreeCount > 4) {
						if (!randmsg)
							multikillmsg = "WICKED SICK";
						else if (randmsg == 1)
							multikillmsg = "MONSTER KILL";
						else
							multikillmsg = "BOOM HEADSHOT";
					}
					else
						break;
					strncat(spam, multikillmsg, sizeof(spam));
					break;
				}
				default:
					break;
			}
			a += 2;
		}
		else
			spam[b] = sNex.ksFormat[a];
		b = strlen(spam);
	}

	sprintf_s(send, sizeof(send), "say \"%s\"\n", spam);
	nexSyscall.SendConsoleCommand(send);
}

void CNexSprees::playSpreeSound() {
	if (!nex.stats.killSpreeCount)
		return;

	sfxHandle_t randomSound;
	int randomNumber;

	switch (nex.stats.killSpreeCount) {
		case 1:
			randomNumber = sizeof(nex.media.spreeSounds.level1) / sizeof(nex.media.spreeSounds.level1[0]);
			if (randomNumber)
				randomNumber--;
			randomSound = nex.media.spreeSounds.level1[rand() % randomNumber];
			break;
		case 2:
			randomNumber = sizeof(nex.media.spreeSounds.level1) / sizeof(nex.media.spreeSounds.level1[0]);
			if (randomNumber)
				randomNumber--;
			randomSound = nex.media.spreeSounds.level2[rand() % randomNumber];
			break;
		case 3:
			randomNumber = sizeof(nex.media.spreeSounds.level1) / sizeof(nex.media.spreeSounds.level1[0]);
			if (randomNumber)
				randomNumber--;
			randomSound = nex.media.spreeSounds.level3[rand() % randomNumber];
			break;
		default:
			randomNumber = sizeof(nex.media.spreeSounds.level1) / sizeof(nex.media.spreeSounds.level1[0]);
			if (randomNumber)
				randomNumber--;
			randomSound = nex.media.spreeSounds.level4[rand() % randomNumber];
			break;
	}

	#ifdef NEX_VERBOSE
		nexUtils.log("Spree: %i | playsound: %i", nex.stats.killSpreeCount, randomSound);
	#endif

	orig_syscall(CG_S_STARTLOCALSOUND, randomSound, CHAN_LOCAL_SOUND, 230);
}