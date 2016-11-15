/* gdvoronoi.cpp */

#include "gdvoronoi.h"
#include "globals.h"
#include "os/os.h"

Voronoi::Voronoi() {
}

void Voronoi::_bind_methods() {
	ObjectTypeDB::bind_method("open", &SQLite::open);
	ObjectTypeDB::bind_method("prepare", &SQLite::prepare);
	ObjectTypeDB::bind_method("step", &SQLite::step);
	ObjectTypeDB::bind_method("step_assoc", &SQLite::step_assoc);
	ObjectTypeDB::bind_method("fetch_assoc", &SQLite::fetch_assoc);
	ObjectTypeDB::bind_method("fetch_one", &SQLite::fetch_one);
	ObjectTypeDB::bind_method("fetch_array", &SQLite::fetch_array);
	ObjectTypeDB::bind_method("query", &SQLite::query);
}

