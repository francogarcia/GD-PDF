/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"

#include "gdpdf.h"

void register_gdpdf_types() {

        ClassDB::register_class<PDF>();
}

void unregister_gdpdf_types() {
   //nothing to do here
}
