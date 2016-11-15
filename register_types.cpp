/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"

#include "gdvoronoi.h"

void register_gdvoronoi_types() {

        ObjectTypeDB::register_type<Voronoi>();
}

void unregister_gdvoronoi_types() {
   //nothing to do here
}
