/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"

#include "gdpdf.h"

void initialize_gdpdf_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(PDF);
	// ClassDB::register_class<PDF>();
}

void uninitialize_gdpdf_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	//nothing to do here
}
