/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"

#include "gdpdf.h"

void register_gdpdf_types() {

        ObjectTypeDB::register_type<PDF>();
}

void unregister_gdpdf_types() {
   //nothing to do here
}
