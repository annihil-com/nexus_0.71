// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexEtpro nexEtpro;
etproAntiCheat_t orig_etproAntiCheat;

void __cdecl nex_etproAntiCheat(void *a, void *b, void *c, int checksum, void *e, char *orig_guid) {
	#ifdef NEX_VERBOSE
		nexUtils.log("etpro AC func called. checksum: %i | guid: %s", checksum, orig_guid);
	#endif

	if (sNex.etproSpoof) {
		orig_etproAntiCheat(a, b, c, checksum, e, sNex.etproGuid);
	} else {
		orig_etproAntiCheat(a, b, c, checksum, e, orig_guid);
	}

	nexEtpro.redirectSyscall();
}

// Redirecting it on CG_INIT and everytime anti-cheat function is called
void CNexEtpro::redirectSyscall() {
	*(DWORD*) sNex.syscallptr = (DWORD) nex_CG_syscall;
}

// Credits to sinner
DWORD HideFromAC(PVOID) {
	// wait a bit till etpro makes checks
	Sleep(500);
	// hook cgame and redirect syscall to our
	nexHook.hookCG(true);
	nexEtpro.redirectSyscall();
	// disable etpro cvar checks
	unprotect(sNex.cgHandle + nex.mod->cvar_proof, 4096);
	memcpy((void *) (sNex.cgHandle + nex.mod->cvar_proof), "\x31", 1);
	reprotect(sNex.cgHandle + nex.mod->cvar_proof, 4096);
	// etpro pwnd
	CloseHandle(nex.hHideFromAC);
	nex.hHideFromAC = NULL;
	return 0;
}
