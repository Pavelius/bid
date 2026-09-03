/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#pragma once

typedef bool(*fnuctest)(int v);

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title = 0, const char* cancel = 0);
unsigned char choosev(unsigned long t1, unsigned long t2, fnuctest condition, const char** names, const char* title = 0, const char* cancel = 0);
