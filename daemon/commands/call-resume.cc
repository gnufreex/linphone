/*
call-resume.cc
Copyright (C) 2016 Belledonne Communications, Grenoble, France 

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "call-resume.h"

CallResume::CallResume():
	DaemonCommand("call-resume", "call-resume <call id>", "Pause a call (pause current if no id is specified).")
{
	addExample(new DaemonCommandExample("call-resume 1",
										"Status: Ok\n\n"
										"Call was resumed"));

	addExample(new DaemonCommandExample("call-resume 2",
										"Status: Error\n"
										"Reason: No call with such id."));

	addExample(new DaemonCommandExample("call-resume",
										"Status: Error\n"
										"Reason: No current call available."));
}

void CallResume::exec(Daemon* app, const char* args)
{
	LinphoneCore *lc = app->getCore();
	int cid;
	LinphoneCall *call = NULL;
	bool current = false;
	if (sscanf(args, "%i", &cid) == 1) {
		call = app->findCall(cid);
		if (call == NULL) {
			app->sendResponse(Response("No call with such id."));
			return;
		}
	} else {
		call = linphone_core_get_current_call(lc);
		current = true;
		if (call == NULL) {
			app->sendResponse(Response("No current call available."));
			return;
		}
	}

	if( linphone_core_resume_call(lc, call) == 0 ) {
		app->sendResponse(Response(current?"Current call was resumed":
										   "Call was resumed", Response::Ok));
	} else {
		app->sendResponse(Response("Error pausing call"));
	}
}
