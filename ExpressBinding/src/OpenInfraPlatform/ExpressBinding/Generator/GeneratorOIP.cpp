/*
  This file is part of TUM Open Infra Platform Early Binding EXPRESS
  Generator, a simple early binding generator for EXPRESS.
  Copyright (c) 2016-2017 Technical University of Munich
  Chair of Computational Modeling and Simulation.

  TUM Open Infra Platform Early Binding EXPRESS Generator is free
  software; you can redistribute it and/or modify it under the terms
  of the GNU General Public License Version 3 as published by the Free
  Software Foundation.

  TUM Open Infra Platform Early Binding EXPRESS Generator is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "GeneratorOIP.h"

#include "OpenInfraPlatform/ExpressBinding/namespace.h"
#include <Windows.h>
#include <algorithm>
#include <cassert>
#include <experimental/filesystem>
#include <fstream>
#include <map>
#include <sstream>

OIP_NAMESPACE_OPENINFRAPLATFORM_EXPRESSBINDING_BEGIN

namespace fs = std::experimental::filesystem;

// https://stackoverflow.com/questions/1528298/get-path-of-executable
std::string executable_path() {
	char buf[1024] = {0};
	DWORD ret = GetModuleFileNameA(NULL, buf, sizeof(buf));
	if (ret == 0 || ret == sizeof(buf)) {
		return "";
	}
	return buf;
}

std::string license =
  "\
\
// Copied and modified code from \"IfcPlusPlus\" library.\n\
// This library is available under the OpenSceneGraph Public License. Original copyright notice:\n\
\n\
/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold\n\
*\n\
* This library is open source and may be redistributed and / or modified under\n\
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or\n\
* (at your option) any later version.The full license is in LICENSE file\n\
* included with this distribution, and on the openscenegraph.org website.\n\
*\n\
* This library is distributed in the hope that it will be useful,\n\
* but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\n\
* OpenSceneGraph Public License for more details.\n\
*/\
";

std::string notice =
  "\
/* This file has been automatically generated using the TUM Open Infra Platform\n\
Early Binding EXPRESS Generator. Any changes to this file my be lost in the future. */";

std::string toUpper(const std::string &inputString) {
	std::string str = inputString;
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

std::string replaceAll(std::string str, const std::string &from, const std::string &to) {
	// replace this using boost!
	// copied from
	// http://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void copyTemplate(const std::string &templateFilename, const std::string &outFilename, const Schema &schema) {
	std::ofstream out(outFilename);

	fs::path executablePath = executable_path();
	
	std::ifstream in(executablePath.parent_path().string() + "/" + templateFilename);
	std::string line;
	while (std::getline(in, line)) {
		out << replaceAll(line, "Ifc2x3", schema.getName()) << std::endl;
	}
}

OpenInfraPlatform::ExpressBinding::GeneratorOIP::GeneratorOIP(const std::string &outputDirectory) : outputDirectory_(outputDirectory) {
}

GeneratorOIP::~GeneratorOIP() {
}

void GeneratorOIP::generate(std::ostream &out, OpenInfraPlatform::ExpressBinding::Schema &schema) {
	rootDirectory_ = outputDirectory_ + "/" + schema.getName();

	std::string schemaDirectory = rootDirectory_ + "/schema";
	sourceDirectory_ = rootDirectory_ + "/src";
	entityPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/entity";
	guidPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/guid";
	readerPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/reader";
	modelPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/model";
	writerPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/writer";
	xmlPath_ = sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName() + "/xml";

	if (!fs::exists(rootDirectory_)) {
		fs::create_directory(rootDirectory_);
	}

	if (!fs::exists(schemaDirectory)) {
		fs::create_directory(schemaDirectory);
	}

	if (!fs::exists(sourceDirectory_)) {
		fs::create_directory(sourceDirectory_);
	}

	if (!fs::exists(sourceDirectory_ + "/OpenInfraPlatform")) {
		fs::create_directory(sourceDirectory_ + "/OpenInfraPlatform");
	}

	if (!fs::exists(sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName())) {
		fs::create_directory(sourceDirectory_ + "/OpenInfraPlatform/" + schema.getName());
	}

	if (!fs::exists(guidPath_)) {
		fs::create_directory(guidPath_);
	}

	if (!fs::exists(entityPath_)) {
		fs::create_directory(entityPath_);
	}

	if (!fs::exists(readerPath_)) {
		fs::create_directory(readerPath_);
	}

	if (!fs::exists(modelPath_)) {
		fs::create_directory(modelPath_);
	}

	if (!fs::exists(writerPath_)) {
		fs::create_directory(writerPath_);
	}

	if (!fs::exists(xmlPath_)) {
		fs::create_directory(xmlPath_);
	}

	// guid
	copyTemplate("Template/guid/CreateGuid_64.h", guidPath_ + "/CreateGuid_64.h", schema);
	copyTemplate("Template/guid/CreateGuid_64.cpp", guidPath_ + "/CreateGuid_64.cpp", schema);
	// model
	copyTemplate("Template/model/Exception.cpp", modelPath_ + "/Exception.cpp", schema);
	copyTemplate("Template/model/Exception.h", modelPath_ + "/Exception.h", schema);
	copyTemplate("Template/model/Model.cpp", modelPath_ + "/Model.cpp", schema);
	copyTemplate("Template/model/Model.h", modelPath_ + "/Model.h", schema);
	copyTemplate("Template/model/Object.cpp", modelPath_ + "/Object.cpp", schema);
	copyTemplate("Template/model/Object.h", modelPath_ + "/Object.h", schema);
	copyTemplate("Template/model/shared_ptr.h", modelPath_ + "/shared_ptr.h", schema);
	copyTemplate("Template/model/StatusObservable.cpp", modelPath_ + "/StatusObservable.cpp", schema);
	copyTemplate("Template/model/StatusObservable.h", modelPath_ + "/StatusObservable.h", schema);
	copyTemplate("Template/model/UnitConverter.cpp", modelPath_ + "/UnitConverter.cpp", schema);
	copyTemplate("Template/model/UnitConverter.h", modelPath_ + "/UnitConverter.h", schema);
	copyTemplate("Template/model/UnknownEntityException.h", modelPath_ + "/UnknownEntityException.h", schema);
	copyTemplate("Template/model/Util.h", modelPath_ + "/Util.h", schema);
	// reader
	copyTemplate("Template/reader/IfcStepReader.cpp", readerPath_ + "/IfcStepReader.cpp", schema);
	copyTemplate("Template/reader/IfcStepReader.h", readerPath_ + "/IfcStepReader.h", schema);
	copyTemplate("Template/reader/IfcXmlReader.cpp", readerPath_ + "/IfcXmlReader.cpp", schema);
	copyTemplate("Template/reader/IfcXmlReader.h", readerPath_ + "/IfcXmlReader.h", schema);
	copyTemplate("Template/reader/Reader.cpp", readerPath_ + "/Reader.cpp", schema);
	copyTemplate("Template/reader/Reader.h", readerPath_ + "/Reader.h", schema);
	copyTemplate("Template/reader/ReaderUtil.cpp", readerPath_ + "/ReaderUtil.cpp", schema);
	copyTemplate("Template/reader/ReaderUtil.h", readerPath_ + "/ReaderUtil.h", schema);
	// writer
	copyTemplate("Template/writer/IfcStepWriter.cpp", writerPath_ + "/IfcStepWriter.cpp", schema);
	copyTemplate("Template/writer/IfcStepWriter.h", writerPath_ + "/IfcStepWriter.h", schema);
	copyTemplate("Template/writer/IfcWriter.h", writerPath_ + "/IfcWriter.h", schema);
	copyTemplate("Template/writer/IfcXmlWriter.cpp", writerPath_ + "/IfcXmlWriter.cpp", schema);
	copyTemplate("Template/writer/IfcXmlWriter.h", writerPath_ + "/IfcXmlWriter.h", schema);
	copyTemplate("Template/writer/WriterUtil.cpp", writerPath_ + "/WriterUtil.cpp", schema);
	copyTemplate("Template/writer/WriterUtil.h", writerPath_ + "/WriterUtil.h", schema);
	// xml
	copyTemplate("Template/xml/XmlTools.cpp", xmlPath_ + "/XmlTools.cpp", schema);
	copyTemplate("Template/xml/XmlTools.h", xmlPath_ + "/XmlTools.h", schema);

	generateCMakeListsFile(schema);

	// Types.h
	createTypesHeaderFile(schema);

	// Entities.h
	createEntitiesHeaderFile(schema);
	createEntitiesMapHeaderFile(schema);

	// EntityBroker.cpp
	createEntityBrokerCPPFile(schema);

	generateEntityEnumsHeaderFile(schema);

	for (int i = 0; i < schema.getTypeCount(); i++) {
		auto &type = schema.getTypeByIndex(i);

		generateTypeHeaderFile(schema, type);
		generateTypeSourceFile(schema, type);
	}

	for (int i = 0; i < schema.getEntityCount(); i++) {
		auto &entity = schema.getEntityByIndex(i);

		generateEntityHeaderFile(schema, entity);
		generateEntitySourceFile(schema, entity);
	}
}

void GeneratorOIP::createEntitiesMapHeaderFile(Schema &schema) {
	// EntitiesMap.h
	std::stringstream ssFilename;
	ssFilename << sourceDirectory_ << "/OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntitiesMap.h";
	std::ofstream out(ssFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << "#pragma once" << std::endl;

	out << "#include <map>" << std::endl;
	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntityEnums.h\" " << std::endl;

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;
	out << "	namespace " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t"
	    << "\t"
	    << "static const std::pair<std::string, " << schema.getName() << "EntityEnum> initializers_" << schema.getName() << "_entity[] = {" << std::endl;

	for (int i = 0; i < schema.getEntityCount(); i++) {
		auto &entity = schema.getEntityByIndex(i);

		out << "\t"
		    << "\t"
		    << "\t"
		    << "std::pair<std::string, " << schema.getName() << "EntityEnum>(\"" << toUpper(entity.getName()) << "\", " << toUpper(entity.getName()) << ")";

		if (i < schema.getEntityCount() - 1) {
			out << ",";
		}

		out << std::endl;
	}

	out << "\t"
	    << "\t"
	    << "};" << std::endl;
	out << "\t"
	    << "}" << std::endl;
	out << "}" << std::endl;
}

void GeneratorOIP::createTypesHeaderFile(Schema &schema) {
	std::stringstream ssFilename;

	ssFilename << sourceDirectory_ << "/OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "Types.h";
	std::ofstream out(ssFilename.str());

	out << license;
	out << std::endl;
	out << "#pragma once" << std::endl;

	for (int i = 0; i < schema.getTypeCount(); i++) {
		auto &type = schema.getTypeByIndex(i);

		out << "#include \"OpenInfraPlatform/" + schema.getName() + "/entity/" << type.getName() << ".h\"" << std::endl;
	}
}

void GeneratorOIP::createEntitiesHeaderFile(Schema &schema) {
	std::stringstream ssFilename;
	ssFilename << sourceDirectory_ << "/OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "Entities.h";
	std::ofstream out(ssFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << "#pragma once" << std::endl;

	for (int i = 0; i < schema.getEntityCount(); i++) {
		auto &entity = schema.getEntityByIndex(i);

		out << "#include \"OpenInfraPlatform/" + schema.getName() + "/entity/" << entity.getName() << ".h\"" << std::endl;
	}
}

void GeneratorOIP::generateTypeHeaderFile(Schema &schema, Type &type) {
	std::stringstream ssHeaderFilename;
	ssHeaderFilename << entityPath_ << "/" << type.getName() << ".h";
	std::cout << ssHeaderFilename.str() << std::endl;
	std::ofstream out(ssHeaderFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << notice << std::endl;
	out << std::endl;

	out << "#pragma once" << std::endl;
	out << std::endl;

	// Includes
	out << "#include <vector>" << std::endl;
	out << "#include <map>" << std::endl;
	out << "#include <memory>" << std::endl;
	out << "#include <sstream>" << std::endl;
	out << "#include <string>" << std::endl;

	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/model/Object.h\"" << std::endl;

	if (!type.isSimpleType() && !type.isEnumeration() && !type.isList() && !type.isArray() && !type.isSet() && !type.isSelectType()) {
		out << "#include \"OpenInfraPlatform/" << schema.getName() << "/entity/" << type.getUnderlyingTypeName() << ".h\"" << std::endl;
	}

	if (type.getName() == "IfcPlaneAngleMeasure") {
		std::cout << "Found IfcPlaneAngleMeasure" << std::endl;
	}

	// collect the class from which this object should derive from
	std::vector<std::string> baseClasses;

	if (true) {
		// very stupid and slow select types lookup...
		for (int i = 0; i < schema.getTypeCount(); ++i) {
			Type t = schema.getTypeByIndex(i);

			if (t.isSelectType()) {
				for (int k = 0; k < t.getTypeCount(); ++k) {
					if (t.getType(k) == type.getName()) {
						out << "#include \"" << t.getName() << ".h\"" << std::endl;

						baseClasses.push_back(t.getName());
					}
				}
			}
		}
	}

	out << std::endl;

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;

	out << "\t"
	    << "namespace"
	    << " " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t"
	    << "// TYPE " << type.getName() << " = " << type.getUnderlyingTypeName() << ";" << std::endl;

	out << "\t"
	    << "\t"
	    << "class " << type.getName();

	if (type.isSimpleType() || type.isArray() || type.isSet() || type.isEnumeration()) {
		baseClasses.push_back(schema.getName() + "Type");
	}

	if (type.isEnumeration()) {
		baseClasses.push_back(schema.getName() + "AbstractEnum");
	}

	if (type.isSelectType()) {
		bool usedInAnotherSelect = false;

		// very stupid and slow select types lookup...
		for (int i = 0; i < schema.getTypeCount(); ++i) {
			Type t = schema.getTypeByIndex(i);

			if (t.isSelectType()) {
				for (int k = 0; k < t.getTypeCount(); ++k) {
					if (t.getType(k) == type.getName()) {
						usedInAnotherSelect = true;
					}
				}
			}
		}

		if (!usedInAnotherSelect) {
			baseClasses.push_back(schema.getName() + "AbstractSelect");
		}
	}

	if (!type.isSimpleType() && !type.isEnumeration() && !type.isList() && !type.isArray() && !type.isSet() && !type.isSelectType()) {
		baseClasses.push_back(type.getUnderlyingTypeName());
	}

	// inherit form base classes
	for (int j = 0; j < baseClasses.size(); j++) {
		if (j == 0) {
			out << " : ";
		}

		out << "public " << baseClasses[j];

		if (baseClasses.size() > j + 1) {
			out << ", ";
		}
	}

	out << std::endl;

	out << "\t"
	    << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t"
	    << "public:" << std::endl;

	if (type.isEnumeration()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "enum " << type.getName() << "Enum" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "{" << std::endl;

		for (int i = 0; i < type.getTypeCount(); ++i) {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t ENUM_" << type.getTypes()[i]; // add _ENUM to avoid collisions with C++
			                                         // identifiers, e.g. "NULL"

			if (i != type.getTypeCount() - 1) {
				out << "," << std::endl;
			}
		}
		out << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "};" << std::endl;
		out << std::endl;
	}

	// ctor

	out << "\t"
	    << "\t"
	    << "\t" << type.getName() << "();" << std::endl;

	// ctor2

	if (type.isEnumeration()) {
		out << "\t"
		    << "\t"
		    << "\t" << type.getName() << "(" << type.getName() << "Enum"
		    << " value);" << std::endl;
	} else if (type.isSimpleType()) {
		out << "\t"
		    << "\t"
		    << "\t" << type.getName() << "(" << convertSimpleTypeToCPPType(schema, type.getUnderlyingTypeName()) << " value);" << std::endl;
	} else if (type.getType() == eType::Unknown) {
		if (schema.isIntegerType(type.getName())) {
			out << "\t"
			    << "\t"
			    << "\t" << type.getName() << "(" << convertSimpleTypeToCPPType(schema, type.getUnderlyingTypeName()) << " value);" << std::endl;
		}
	} else if (type.isList()) {
		std::cout << "skip it" << std::endl;
		// skip it!
	} else {
		std::cout << "mu" << std::endl;
	}

	// dtor
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual ~" << type.getName() << "();" << std::endl;

	// classname
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual const char* classname() const { return \"" << type.getName() << "\"; }" << std::endl;

	// getStepParamter
	if (type.isSelectType()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "virtual void getStepParameter(std::stringstream& stream, bool "
		       "is_select_type = false) const = 0; // abstract class"
		    << std::endl;
	} else {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "virtual void getStepParameter(std::stringstream& stream, bool "
		       "is_select_type = false) const;"
		    << std::endl;
	}

	// readStepData
	if (type.isSelectType()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "static std::shared_ptr<" << type.getName() << "> readStepData(const std::string& arg, const std::map<int,shared_ptr<" << schema.getName() << "Entity> >& map);"
		    << std::endl;
	} else {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "static std::shared_ptr<" << type.getName() << "> readStepData(const std::string& arg);" << std::endl;
	}

	// attributes
	if (type.isSimpleType()) {
		out << "\t"
		    << "\t"
		    << "\t" << convertSimpleTypeToCPPType(schema, type.getUnderlyingTypeName()) << " m_value;" << std::endl;
	}

	if (type.isList()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "std::vector<" << convertSimpleTypeToCPPType(schema, type.getContainerType()) << "> m_vec;" << std::endl;
	}

	if (type.isArray()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "std::vector<" << convertSimpleTypeToCPPType(schema, type.getContainerType()) << "> m_vec;" << std::endl;
	}

	if (type.isEnumeration()) {
		out << "\t"
		    << "\t"
		    << "\t" << type.getName() << "Enum"
		    << " m_enum;" << std::endl;
	}

	out << "\t"
	    << "\t"
	    << "}; // end class " << type.getName() << std::endl;

	out << "\t"
	    << "} // end namespace " << schema.getName() << std::endl;

	out << "} // end namespace OpenInfraPlatform" << std::endl;
}

void GeneratorOIP::generateTypeSourceFile(Schema &schema, Type &type) {
	std::string name = type.getName();

	if (name == "IfcLabel") {
		std::cout << "found";
	}

	std::stringstream ssFilename;
	ssFilename << entityPath_ << "/" << name << ".cpp";
	std::ofstream out(ssFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << notice << std::endl;
	out << std::endl;

	out << "#include \"" << type.getName() << ".h\"" << std::endl;
	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/reader/ReaderUtil.h\"" << std::endl;

	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/writer/WriterUtil.h\"" << std::endl;

	out << std::endl;
	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;

	out << "\t"
	    << "namespace"
	    << " " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	// ctor
	out << "\t"
	    << "\t" << type.getName() << "::" << type.getName() << "() {}" << std::endl;

	// ctor2
	if (type.isEnumeration()) {
		out << "\t"
		    << "\t" << type.getName() << "::" << type.getName() << "(" << type.getName() << "::" << type.getName() << "Enum"
		    << " value) { m_enum = value; }" << std::endl;
	} else if (type.isList()) {
		// skip it
	} else if (type.isSimpleType()) {
		out << "\t"
		    << "\t" << type.getName() << "::" << type.getName() << "(" << convertSimpleTypeToCPPType(schema, type.getUnderlyingTypeName()) << " value) { m_value = value; }"
		    << std::endl;
	} else if (type.getType() == eType::Unknown) {
		if (schema.isIntegerType(type.getName())) {
			out << "\t"
			    << "\t" << type.getName() << "::" << type.getName() << "(" << convertSimpleTypeToCPPType(schema, type.getUnderlyingTypeName()) << " value) { m_value = value; }"
			    << std::endl;
		}
	} else {
	}

	// dtor
	out << "\t"
	    << "\t" << type.getName() << "::~" << type.getName() << "() {}" << std::endl;

	// getStepParameter
	generateTypeSourceFileGetStepParameter(type, out, schema);

	// readStepData
	if (type.isSelectType()) {
		out << "\t"
		    << "\t"
		    << "std::shared_ptr<" << type.getName() << "> " << type.getName() << "::readStepData(const std::string& arg, const std::map<int,shared_ptr<" << schema.getName()
		    << "Entity> >& map)" << std::endl;
		out << "\t"
		    << "\t"
		    << "{" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "// Read SELECT TYPE" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "if( arg.size() == 0 ) { return std::shared_ptr<" << type.getName() << ">(); }" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "if( arg[0] == '#' )" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "int id=atoi( arg.substr(1,arg.length()-1).c_str() );" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "std::map<int,shared_ptr<" << schema.getName() << "Entity> >::const_iterator it_entity = map.find( id );" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "if( it_entity != map.end() )" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t\t\t\t\t"
		    << "shared_ptr<" << type.getName() << "> type_object = dynamic_pointer_cast<" << type.getName() << ">(it_entity->second);" << std::endl;
		out << "\t\t\t\t\t"
		    << "return type_object;" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "}" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "else" << std::endl;
		out << "\t\t\t\t"
		    << "{" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "std::stringstream strs;" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "strs << \"Object width id \" << id << \" not found\";" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "throw " << schema.getName() << "Exception( strs.str() );" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "}" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "}" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "else if( arg.compare(\"$\")==0 )" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "return shared_ptr<" << type.getName() << ">();" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "}" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "else if( arg.compare(\"*\")==0 )" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "return shared_ptr<" << type.getName() << ">();" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "}" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "else" << std::endl;
		out << "\t\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "// inline arguments" << std::endl;

		out << "\t\t"
		    << "\t\t"
		    << "std::string keyword;" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "std::string inline_arg;" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "tokenizeInlineArgument( arg, keyword, inline_arg );" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "std::stringstream strs;" << std::endl;
		out << "\t\t"
		    << "\t\t"
		    << "strs << \"unhandled inline argument: \" << arg << \" in function " << schema.getName() << "::" << type.getName() << "::readStepData\" << std::endl;" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "throw " << schema.getName() << "Exception( strs.str() );" << std::endl;

		out << "\t\t"
		    << "\t"
		    << "}" << std::endl;

		out << "\t"
		    << "\t\t"
		    << "return shared_ptr<" << type.getName() << ">();" << std::endl;

		out << "\t"
		    << "\t"
		    << "}" << std::endl;
	} else if (type.isEnumeration()) {
		out << "\t"
		    << "\t"
		    << "\t"
		    << "std::shared_ptr<" << type.getName() << "> " << type.getName() << "::readStepData(const std::string& arg)" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "if( arg.compare( \"$\" ) == 0 ) { return std::shared_ptr<" << type.getName() << ">(); }" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "std::shared_ptr<" << type.getName() << "> type_object(new " << type.getName() << "() );" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "if ( _stricmp( arg.c_str(), \"." << type.getTypes()[0] << ".\") == 0)" << std::endl;

		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "type_object->m_enum = " << type.getName() << "::" << type.getName() << "Enum::ENUM_" << type.getTypes()[0] << ";" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "}" << std::endl;

		for (int i = 1; i < type.getTypes().size(); i++) {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "else if ( _stricmp( arg.c_str(), \"." << type.getTypes()[i] << ".\") == 0)" << std::endl;
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "{" << std::endl;
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "type_object->m_enum = " << type.getName() << "::" << type.getName() << "Enum::ENUM_" << type.getTypes()[i] << ";" << std::endl;
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "}" << std::endl;
		}

		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "return type_object;" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "}" << std::endl;
	} else {
		out << "\t"
		    << "\t"
		    << "std::shared_ptr<" << type.getName() << "> " << type.getName() << "::readStepData(const std::string& arg)" << std::endl;
		out << "\t"
		    << "\t"
		    << "{" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "if( arg.compare( \"$\" ) == 0 ) { return std::shared_ptr<" << type.getName() << ">(); }" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "else if( arg.compare( \"*\" ) == 0 ) { return std::shared_ptr<" << type.getName() << ">(); }" << std::endl;
		out << "\t"
		    << "\t"
		    << "\t"
		    << "std::shared_ptr<" << type.getName() << "> type_object(new " << type.getName() << "() );" << std::endl;

		if (type.isList()) {
			if (type.getContainerType() == "IfcPositiveInteger") {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "    readIntList(  arg, type_object->m_vec );" << std::endl;
			}
		} else if (type.isSimpleType()) {
			if (type.getType() == eType::Real) {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "\t"
				    << "type_object->m_value = atof(arg.c_str());" << std::endl;
			} else if (type.getType() == eType::Boolean) {
				out << "\t\t\t\t"
				    << "if (_stricmp(arg.c_str(), \".F.\") == 0)" << std::endl;
				out << "\t\t\t\t"
				    << "{" << std::endl;
				out << "\t\t\t\t"
				    << "	type_object->m_value = false;" << std::endl;
				out << "\t\t\t\t"
				    << "}" << std::endl;
				out << "\t\t\t\t"
				    << "else if (_stricmp(arg.c_str(), \".T.\") == 0)" << std::endl;
				out << "\t\t\t\t"
				    << "{" << std::endl;
				out << "\t\t\t\t"
				    << "	type_object->m_value = true;" << std::endl;
				out << "\t\t\t\t"
				    << "}" << std::endl;
			} else if (type.getType() == eType::Binary) {
				// ignore it...
			} else if (type.getType() == eType::Integer || type.getType() == eType::Number) {
				out << "\t\t\t"
				    << "type_object->m_value = atoi(arg.c_str());" << std::endl;
			} else if (type.getType() == eType::String) {
				out << "\t\t\t"
				    << "type_object->m_value = arg;" << std::endl;
			}

		} else if (type.isArray()) {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "\t"
			    << "readDoubleList( arg, type_object->m_vec );" << std::endl;
		} else {
			if (schema.isRealType(type.getName())) {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "type_object->m_value = atof(arg.c_str());" << std::endl;
			}
		}
		out << "\t"
		    << "\t"
		    << "\t"
		    << "return type_object;" << std::endl;
		out << "\t"
		    << "\t"
		    << "}" << std::endl;
	}

	out << "\t"
	    << "} // end namespace " << schema.getName() << std::endl;

	out << "} // end namespace OpenInfraPlatform" << std::endl;
}

bool isSimpleType(std::string name) {
	if (name == "REAL")
		return true;
	if (name == "BINARY")
		return true;
	if (name == "BOOLEAN")
		return true;
	if (name == "INTEGER")
		return true;
	if (name == "STRING")
		return true;
	if (name == "LOGICAL")
		return true;
	if (name == "NUMBER")
		return true;

	return false;
}

void OpenInfraPlatform::ExpressBinding::GeneratorOIP::generateTypeSourceFileGetStepParameter(const Type &type, std::ofstream &out, Schema &schema) {
	if (type.isSelectType()) {
		return;
	}

	out << "\t\t"
	    << "void " << type.getName()
	    << "::getStepParameter(std::stringstream&"
	       " stream, bool is_select_type) const"
	    << std::endl;

	if (type.isEnumeration()) {
		out << "\t\t\t"
		    << "{" << std::endl;

		out << "\t\t\t\t"
		    << "if ( m_enum == " << type.getName() << "Enum::ENUM_" << type.getTypes()[0] << ")" << std::endl;

		out << "\t\t\t\t"
		    << "{" << std::endl;
		out << "\t\t\t\t\t"
		    << "stream << \"." << type.getTypes()[0] << ".\";" << std::endl;
		out << "\t\t\t"
		    << "\t"
		    << "}" << std::endl;

		for (int i = 1; i < type.getTypes().size(); i++) {
			out << "\t\t\t\t"
			    << "else if ( m_enum == " << type.getName() << "Enum::ENUM_" << type.getTypes()[i] << ")" << std::endl;
			out << "\t\t\t\t"
			    << "{" << std::endl;
			out << "\t\t\t\t\t"
			    << "stream << \"." << type.getTypes()[i] << ".\";" << std::endl;
			out << "\t\t\t\t"
			    << "}" << std::endl;
		}

		out << "\t\t\t\t"
		    << "if (is_select_type) { stream << \")\"; }" << std::endl;
		out << "\t\t\t"
		    << "}" << std::endl;

		return;
	}

	out << "\t\t"
	    << "{" << std::endl;
	out << "\t\t\t"
	    << "if (is_select_type) { stream << \"" << toUpper(type.getName()) << "(\"; }" << std::endl;

	if (type.isList()) {
		if (type.getContainerType() == "IfcPositiveInteger") {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "writeIntList( stream, m_vec );" << std::endl;
		}
	}

	if (!type.isSimpleType() && !type.isList() && !type.isEnumeration() && !type.isArray() && !type.isSet() && !type.isSelectType()) {
		Type currentType = schema.getTypeByName(type.getUnderlyingTypeName());

		while (!currentType.isSimpleType()) {
			currentType = schema.getTypeByName(currentType.getUnderlyingTypeName());
		}

		if (currentType.getUnderlyingTypeName() == "STRING") {
			out << "\t"
			    << "\t"
			    << "stream << \"'\" << encodeStepString( m_value ) << "
			       "\"'\";"
			    << std::endl;
		}
	}

	if (type.getType() == eType::Unknown) {
		// must be derived form some simple type
		std::string underlyingType = type.getUnderlyingTypeName();

		while (!isSimpleType(underlyingType)) {
			const Type &ut = schema.getTypeByName(underlyingType);
			underlyingType = ut.getUnderlyingTypeName();
		}

		if (underlyingType == "REAL") {
			out << "\t\t\t"
			    << "stream << m_value;" << std::endl;
		} else if (underlyingType == "STRING") {
			out << "\t\t\t"
			    << "stream << \"'\" << encodeStepString( m_value ) << \"'\";" << std::endl;
		} else if (underlyingType == "INTEGER") {
			out << "\t\t\t"
			    << "stream << m_value;" << std::endl;
		} else {
			std::cout << "mu" << std::endl;
		}
	}

	if (type.isSimpleType()) {
		if (type.getType() == eType::Integer) {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "  stream << m_value;" << std::endl;
		} else if (type.getType() == eType::Boolean) {
			out << "\t\t\t"
			    << "if (m_value == false)" << std::endl;
			out << "\t\t\t"
			    << "{" << std::endl;
			out << "\t\t\t"
			    << "	stream << \".F.\";" << std::endl;
			out << "\t\t\t"
			    << "}" << std::endl;
			out << "\t\t\t"
			    << "else if (m_value == true)" << std::endl;
			out << "\t\t\t"
			    << "{" << std::endl;
			out << "\t\t\t"
			    << "	stream << \".T.\";" << std::endl;
			out << "\t\t\t"
			    << "}" << std::endl;
		} else if (type.getType() == eType::String) {
			out << "\t\t\t"
			    << "stream << \"'\" << encodeStepString( m_value ) << \"'\";" << std::endl;
		} else if (type.getType() == eType::Real) {
			out << "\t\t\t"
			    << "stream << m_value;" << std::endl;
		} else {
			std::cout << "muh" << std::endl;
		}
	}

	if (type.isArray()) {
		if (type.getContainerType() == "REAL") {
			out << "\t\t\t  "
			    << "writeDoubleList( stream, m_vec );" << std::endl;
		}
	}

	out << "\t\t\t"
	    << "if (is_select_type) { stream << \")\"; }" << std::endl;
	out << "\t\t"
	    << "}" << std::endl;
}

std::string GeneratorOIP::convertSimpleTypeToCPPType(Schema &schema, std::string simpleType) const {
	std::map<std::string, std::string> simpleTypeMapping;
	simpleTypeMapping.insert(std::make_pair("INTEGER", "int"));
	simpleTypeMapping.insert(std::make_pair("REAL", "double"));
	simpleTypeMapping.insert(std::make_pair("NUMBER", "int"));
	simpleTypeMapping.insert(std::make_pair("STRING", "std::string"));
	simpleTypeMapping.insert(std::make_pair("BOOLEAN", "bool"));
	simpleTypeMapping.insert(std::make_pair("LOGICAL", "bool"));
	simpleTypeMapping.insert(std::make_pair("BINARY", "const char*"));
	// simpleTypeMapping.insert(std::make_pair("IfcPositiveInteger", "int"));

	if (!simpleTypeMapping.count(simpleType)) {
		std::cout << simpleType << std::endl;

		Type underlyingType = schema.getTypeByName(simpleType);
		while (!underlyingType.isSimpleType()) {
			underlyingType = schema.getUnderlyingType(underlyingType.getName());
		}

		return convertSimpleTypeToCPPType(schema, underlyingType.getUnderlyingTypeName());
	}

	assert(simpleTypeMapping.count(simpleType));

	return simpleTypeMapping[simpleType];
}

void GeneratorOIP::includeFile(const std::string &filename, std::ofstream &file) {
	std::ifstream in(filename);
	std::string line;
	while (std::getline(in, line)) {
		file << line << std::endl;
	}
}

void GeneratorOIP::generateCMakeListsFile(const Schema &schema) {
	std::string name = "CMakeLists.txt";

	std::stringstream ssFilename;
	ssFilename << rootDirectory_ << "/" << name;
	std::ofstream file(ssFilename.str());

	// file << "cmake_minimum_required(VERSION 3.5)" << std::endl;

	file << "project(OpenInfraPlatform." << schema.getName() << ")" << std::endl;
	file << "" << std::endl;
	file << "set(CMAKE_DEBUG_POSTFIX \"d\")" << std::endl << std::endl;

	file << "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	../Debug)" << std::endl;
	file << "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	../Release)" << std::endl;
	file << "set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG 	    ../Debug)" << std::endl;
	file << "set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE 	    ../Release)" << std::endl;

	// file << "find_package(Boost 1.61.0 REQUIRED)" << std::endl;

	// file << "# Compiler settings" << std::endl;
	// file << "if (WIN32)" << std::endl;
	// file << "# Standard: / DWIN32 / D_WINDOWS / W3 / Zm1000 / EHsc / GR"
	//     << std::endl;
	// file << "	set(CMAKE_CXX_FLAGS \"/DWIN32 /D_WINDOWS /W4 /Zi /EHsc /GR /MP "
	//        "/openmp /bigobj\")"
	//     << std::endl;
	// file << "	endif(WIN32)" << std::endl;

	file << "" << std::endl;
	file << "include_directories(" << std::endl;
	file << "  src/OpenInfraPlatform/" << schema.getName() << std::endl;
	file << "  src" << std::endl;
	file << "  ${Boost_INCLUDE_DIR}" << std::endl;
	file << ")" << std::endl;
	file << "" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_Source              "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_guid_Source         "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/guid/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_entity_Source         "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/entity/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_include_Source "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/entity/include/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_model_Source        "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/model/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_reader_Source       "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/reader/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_writer_Source       "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/writer/*.*)" << std::endl;
	file << "file(GLOB OpenInfraPlatform_" << schema.getName()
	     << "_xml_Source          "
	        "src/OpenInfraPlatform/"
	     << schema.getName() << "/xml/*.*)" << std::endl;
	file << "" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "                FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\guid            FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_guid_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\entity          FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_entity_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\entity\\\\include FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_include_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\model           FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_model_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\reader          FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_reader_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\writer          FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_writer_Source})" << std::endl;
	file << "source_group(OpenInfraPlatform\\\\" << schema.getName()
	     << "\\\\xml             FILES "
	        "${OpenInfraPlatform_"
	     << schema.getName() << "_xml_Source})" << std::endl;
	file << "" << std::endl;
	file << "add_library(OpenInfraPlatform." << schema.getName() << " STATIC" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_guid_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_entity_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_include_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_model_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_reader_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_writer_Source}" << std::endl;
	file << "\t"
	     << "${OpenInfraPlatform_" << schema.getName() << "_xml_Source}" << std::endl;
	file << ")" << std::endl;
	file << "" << std::endl;
	file << "if (WIN32)" << std::endl;
	file << "\t"
	     << "target_link_libraries(OpenInfraPlatform." << schema.getName() << std::endl;
	file << "\t"
	     << "Rpcrt4.lib)" << std::endl;
	file << "endif (WIN32)" << std::endl;
}

void GeneratorOIP::createEntityBrokerCPPFile(Schema &schema) {
	std::stringstream ssFilename;
	ssFilename << sourceDirectory_ << "/OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntityBroker.cpp";
	std::ofstream out(ssFilename.str());

	out << license << std::endl;
	out << std::endl;

	out << "#include <vector>" << std::endl;
	out << "#include <map>" << std::endl;
	out << "#include <string>" << std::endl;
	out << "#include \"" << schema.getName() << "Entities.h\"" << std::endl;
	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntityEnums.h\" " << std::endl;
	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/model/Exception.h\" " << std::endl;
	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/model/Object.h\" " << std::endl;

	out << std::endl;

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;
	out << "	namespace " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t" << schema.getName() << "Entity* create" << schema.getName() << "Entity( const " << schema.getName()
	    << "EntityEnum entity_enum "
	       ")"
	    << std::endl;

	out << "\t"
	    << "\t"
	    << "{" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "switch( entity_enum )" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "{" << std::endl;

	for (int i = 0; i < schema.getEntityCount(); i++) {
		auto &entity = schema.getEntityByIndex(i);

		out << "\t"
		    << "\t"
		    << "\t"
		    << "\t"
		    << "case " << toUpper(entity.getName()) << ": return new " << entity.getName() << "();" << std::endl;
	}

	out << "\t"
	    << "\t"
	    << "\t"
	    << "\t"
	    << "default: throw " << schema.getName() << "Exception(\"given " << schema.getName() << "EntityEnum not valid\"); " << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "}" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "return 0;" << std::endl;

	out << "\t"
	    << "\t"
	    << "}" << std::endl;
	out << "\t"
	    << "}" << std::endl;
	out << "}" << std::endl;
}

void GeneratorOIP::generateEntityHeaderFile(OpenInfraPlatform::ExpressBinding::Schema &schema, Entity &entity) {
	std::stringstream ssHeaderFilename;
	ssHeaderFilename << entityPath_ << "/" << entity.getName() << ".h";
	std::cout << ssHeaderFilename.str() << std::endl;
	std::ofstream out(ssHeaderFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << notice << std::endl;
	out << std::endl;

	out << "#pragma once" << std::endl;
	out << std::endl;

	// Includes
	out << "#include <vector>" << std::endl;
	out << "#include <map>" << std::endl;
	out << "#include <memory>" << std::endl;
	out << "#include <sstream>" << std::endl;
	out << "#include <string>" << std::endl;

	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/model/Object.h\"" << std::endl;

	// very stupid and slow select types lookup...
	for (int i = 0; i < schema.getTypeCount(); ++i) {
		Type t = schema.getTypeByIndex(i);

		if (t.isSelectType()) {
			for (int k = 0; k < t.getTypeCount(); ++k) {
				if (t.getType(k) == entity.getName()) {
					out << "#include \"" << t.getName() << ".h\"" << std::endl;
				}
			}
		}
	}

	if (entity.hasSupertype()) {
		out << "#include \"" << entity.getSupertype() << ".h\"" << std::endl;
	}

	out << std::endl;

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;

	out << "\t"
	    << "namespace"
	    << " " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	for (int i = 0; i < entity.getAttributeCount(); i++) {
		const EntityAttribute &att = entity.getAttribute(i);

		if (att.getParamterType() == eEntityAttributeParamterType::TypeNamed) {
			out << "\t"
			    << "\t"
			    << "class " << att.type->toString() << ";" << std::endl;
		} else if (att.getParamterType() == eEntityAttributeParamterType::eGeneralizedType) {
			auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(att.type);

			if (p->containerType == eEntityAttributeContainerType::List || p->containerType == eEntityAttributeContainerType::Set
			    || p->containerType == eEntityAttributeContainerType::Array) {
				auto iterator = p->elementType;

				while (iterator->getType() == eEntityAttributeParamterType::eGeneralizedType) {
					auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(iterator);

					iterator = p->elementType;
				}

				if (iterator->getType() == eEntityAttributeParamterType::TypeNamed) {
					out << "\t"
					    << "\t"
					    << "class " << iterator->toString() << ";" << std::endl;
				}

				iterator = p->elementType;

			} else {
				std::cout << "lost" << std::endl;
			}
		} else if (att.getParamterType() == eEntityAttributeParamterType::Simple) {
			// std::cout << "m?" << std::endl;

			// out << "\t" << "\t" << "class " << p->elementType->toString() <<
			// ";" << std::endl;
		} else {
			// std::cout << "m?" << std::endl;
		}
	}

	out << "\t"
	    << "\t"
	    << "// ENTITY " << entity.getName() << std::endl;

	out << "\t"
	    << "\t"
	    << "class " << entity.getName();

	if (entity.hasSupertype()) {
		out << " : public " << entity.getSupertype();
	} else {
		out << " : public " << schema.getName() << "Entity";
	}

	// very stupid and slow select types lookup...
	for (int i = 0; i < schema.getTypeCount(); ++i) {
		Type t = schema.getTypeByIndex(i);

		if (t.isSelectType()) {
			for (int k = 0; k < t.getTypeCount(); ++k) {
				if (t.getType(k) == entity.getName()) {
					out << ", public " << t.getName() << std::endl;
				}
			}
		}
	}

	out << std::endl;

	out << "\t"
	    << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t"
	    << "public:" << std::endl;

	// ctors
	out << "\t"
	    << "\t"
	    << "\t" << entity.getName() << "();" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t" << entity.getName() << "( int id );" << std::endl;
	// dtor
	out << "\t"
	    << "\t"
	    << "\t"
	    << "~" << entity.getName() << "();" << std::endl;

	out << std::endl;

	out << "\t"
	    << "\t"
	    << "\t"
	    << "// method setEntity takes over all attributes from another "
	       "instance of the class"
	    << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void setEntity(std::shared_ptr<" << schema.getName() << "Entity> other);" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void getStepLine(std::stringstream& stream) const;" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void getStepParameter(std::stringstream& stream, bool "
	       "is_select_type = false) const;"
	    << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void readStepData(std::vector<std::string>& args, const "
	       "std::map<int, shared_ptr<"
	    << schema.getName() << "Entity> >& map);" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void setInverseCounterparts(shared_ptr<" << schema.getName() << "Entity> ptr_self);" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual void unlinkSelf();" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "virtual const char* classname() const { return \"" << entity.getName() << "\"; }" << std::endl;

	out << std::endl;

	// current class
	out << "\t"
	    << "\t"
	    << "\t"
	    << "// " << entity.getName() << " -----------------------" << std::endl;
	out << "\t"
	    << "\t"
	    << "\t"
	    << "// attributes:" << std::endl;

	for (int i = 0; i < entity.getAttributeCount(); i++) {
		const EntityAttribute &att = entity.getAttribute(i);

		if (att.getParamterType() == eEntityAttributeParamterType::TypeNamed) {
			out << "\t"
			    << "\t"
			    << "\t"
			    << "std::shared_ptr<" << att.type->toString() << "> m_" << att.getName() << ";" << std::endl;
		} else if (att.getParamterType() == eEntityAttributeParamterType::eGeneralizedType) {
			auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(att.type);

			if (p->containerType == eEntityAttributeContainerType::List) {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "std::vector<";

				auto iterator = p->elementType;

				while (iterator->getType() == eEntityAttributeParamterType::eGeneralizedType) {
					auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(iterator);
					out << "std::vector<";

					iterator = p->elementType;
				}

				if (iterator->getType() == eEntityAttributeParamterType::TypeNamed) {
					out << "std::shared_ptr<" << iterator->toString() << ">";
				}

				iterator = p->elementType;

				while (iterator->getType() == eEntityAttributeParamterType::eGeneralizedType) {
					auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(iterator);
					out << ">";

					iterator = p->elementType;
				}

				out << "> "
				    << " m_" << att.getName() << ";" << std::endl;
			} else if (p->containerType == eEntityAttributeContainerType::Set) {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "std::vector<";

				if (p->elementType->getType() == eEntityAttributeParamterType::TypeNamed) {
					out << "std::shared_ptr<" << p->elementType->toString() << ">";
				}

				out << "> "
				    << " m_" << att.getName() << ";" << std::endl;
			} else if (p->containerType == eEntityAttributeContainerType::Array) {
				out << "\t"
				    << "\t"
				    << "\t"
				    << "std::vector<";

				if (p->elementType->getType() == eEntityAttributeParamterType::TypeNamed) {
					out << "std::shared_ptr<" << p->elementType->toString() << ">";
				}

				out << "> "
				    << " m_" << att.getName() << ";" << std::endl;
			} else {
				std::cout << "\t"
				          << "\t"
				          << "\t" << att.type->toString() << " m_" << att.getName() << ";" << std::endl;
				out << "\t"
				    << "\t"
				    << "\t" << att.type->toString() << " m_" << att.getName() << ";" << std::endl;
			}

		} else {
			out << "\t"
			    << "\t"
			    << "\t" << convertSimpleTypeToCPPType(schema, att.type->toString()) << " m_" << att.getName() << ";" << std::endl;
		}
	}

	out << "\t"
	    << "\t"
	    << "};" << std::endl;
	out << "\t"
	    << "}" << std::endl;
	out << "}" << std::endl;
}

void GeneratorOIP::generateEntityEnumsHeaderFile(Schema &schema) {
	std::stringstream ssHeaderFilename;
	ssHeaderFilename << sourceDirectory_ << "/OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntityEnums.h";
	std::cout << ssHeaderFilename.str() << std::endl;
	std::ofstream out(ssHeaderFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << notice << std::endl;
	out << std::endl;

	out << "#pragma once" << std::endl;
	out << std::endl;

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;

	out << "\t"
	    << "namespace"
	    << " " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	out << "\t"
	    << "\t"
	    << "enum " << schema.getName() << "EntityEnum" << std::endl;
	out << "\t"
	    << "\t"
	    << "{" << std::endl;

	for (int i = 0; i < schema.getEntityCount(); i++) {
		out << "\t"
		    << "\t"
		    << "\t" << toUpper(schema.getEntityByIndex(i).getName());
		if (i < schema.getEntityCount() - 1) {
			out << ",";
		}

		out << std::endl;
	}

	out << "\t"
	    << "\t"
	    << "};" << std::endl;

	out << "\t"
	    << "}" << std::endl;
	out << "}" << std::endl;
}

void GeneratorOIP::generateEntitySourceFile(Schema &schema, const Entity &entity) {
	std::stringstream ssHeaderFilename;
	ssHeaderFilename << entityPath_ << "/" << entity.getName() << ".cpp";
	std::cout << ssHeaderFilename.str() << std::endl;
	std::ofstream out(ssHeaderFilename.str());

	out << license << std::endl;
	out << std::endl;
	out << notice << std::endl;
	out << std::endl;

	// Includes
	out << "#include <sstream>" << std::endl;
	out << "#include <limits>" << std::endl;

	out << "#include \"OpenInfraPlatform/" << schema.getName() << "/model/Exception.h\"" << std::endl
	    << "#include \"OpenInfraPlatform/" << schema.getName() << "/reader/ReaderUtil.h\"" << std::endl
	    << "#include \"OpenInfraPlatform/" << schema.getName() << "/writer/WriterUtil.h\"" << std::endl
	    << "#include \"OpenInfraPlatform/" << schema.getName() << "/" << schema.getName() << "EntityEnums.h\"" << std::endl;

	out << "#include \"" << entity.getName() << ".h\"" << std::endl;

	auto attirbuteVector = schema.getAllEntityAttributes(entity);

	for (int i = 0; i < attirbuteVector.size(); i++) {
		const EntityAttribute &att = attirbuteVector[i];

		if (att.getParamterType() == eEntityAttributeParamterType::TypeNamed) {
			const EntityAttribute &att = attirbuteVector[i];
			out << "#include \"" << att.type->toString() << ".h\"" << std::endl;
		} else if (att.getParamterType() == eEntityAttributeParamterType::eGeneralizedType) {
			auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(att.type);

			if (p->containerType == eEntityAttributeContainerType::List || p->containerType == eEntityAttributeContainerType::Set
			    || p->containerType == eEntityAttributeContainerType::Array) {
				if (p->elementType->getType() == eEntityAttributeParamterType::eGeneralizedType) {
					auto q = std::static_pointer_cast<EntityAttributeGeneralizedType>(p->elementType);

					std::cout << q->elementType->toString() << std::endl;
					out << "#include \"" << q->elementType->toString() << ".h\"" << std::endl;
				} else
					out << "#include \"" << p->elementType->toString() << ".h\"" << std::endl;
			}
		}
	}

	out << "namespace OpenInfraPlatform" << std::endl;
	out << "{" << std::endl;

	out << "\t"
	    << "namespace"
	    << " " << schema.getName() << std::endl;
	out << "\t"
	    << "{" << std::endl;

	// ctor impl
	out << "\t\t\t"
	    << "// ENTITY " << entity.getName() << std::endl;
	out << "\t\t\t" << entity.getName() << "::" << entity.getName() << "() { m_entity_enum = " << toUpper(entity.getName()) << ";}" << std::endl;
	out << "\t\t\t" << entity.getName() << "::" << entity.getName() << "( int id ) { m_id = id; m_entity_enum = " << toUpper(entity.getName()) << ";}" << std::endl;
	// dtor impl
	out << "\t\t\t" << entity.getName() << "::~" << entity.getName() << "() {}" << std::endl;

	out << std::endl;

	// setEntity
	out << "\t\t\t"
	    << "// method setEntity takes over all attributes from another "
	       "instance of the class"
	    << std::endl;
	out << "\t\t\t"
	    << "void " << entity.getName() << "::setEntity( std::shared_ptr<" << schema.getName() << "Entity> other_entity)" << std::endl;
	out << "\t\t\t"
	    << "{" << std::endl;

	out << "\t\t\t\t"
	    << "std::shared_ptr<" << entity.getName() << "> other = std::dynamic_pointer_cast<" << entity.getName() << ">(other_entity);" << std::endl;
	out << "\t\t\t\t"
	    << "if( !other) { return; }" << std::endl;

	auto superTypes = schema.getSuperTypes(entity);

	auto attributes = schema.getAllEntityAttributesNames(entity);

	for (int ai = 0; ai < attributes.size(); ++ai) {
		out << "\t\t\t\t"
		    << "m_" << attributes[ai] << " = other->"
		    << "m_" << attributes[ai] << ";" << std::endl;
	}

	out << "\t\t\t"
	    << "}" << std::endl;

	// getStepLine
	out << "\t\t\t"
	    << "void " << entity.getName() << "::getStepLine( std::stringstream& stream ) const" << std::endl;
	out << "\t\t\t"
	    << "{" << std::endl;
	out << "\t\t\t\t"
	    << "stream << \"#\" << m_id << \"=" << toUpper(entity.getName()) << "\" << \"(\";" << std::endl;

	if (entity.getName() == "IfcTriangulatedFaceSet") {
		std::cout << "mu";
	}

	for (int i = 0; i < attirbuteVector.size(); i++) {
		const EntityAttribute &att = attirbuteVector[i];

		if (att.getParamterType() == eEntityAttributeParamterType::eGeneralizedType) {
			auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(att.type);

			std::cout << entity.getName() << "." << att.getName() << std::endl;

			if (p->containerType == eEntityAttributeContainerType::List || p->containerType == eEntityAttributeContainerType::Set
			    || p->containerType == eEntityAttributeContainerType::Array) {
				if (p->elementType->getType() == eEntityAttributeParamterType::eGeneralizedType) {
					// 2D for example List of List
					auto q = std::static_pointer_cast<EntityAttributeGeneralizedType>(p->elementType);

					if (schema.hasType(q->elementType->toString())) {
						if (schema.isIntegerType(q->elementType->toString())) {
							out << "\t\t\t\t"
							    << "writeTypeOfIntList2D( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
						} else {
							out << "\t\t\t\t"
							    << "writeTypeOfRealList2D( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
						}

					} else {
						out << "\t\t\t\t"
						    << "writeEntityList2D( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
					}
				} else {
					if (schema.hasType(p->elementType->toString())) {
						// is type a Real Type?
						if (schema.isRealType(p->elementType->toString())) {
							out << "\t\t\t\t"
							    << "writeTypeOfRealList( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
						} else {
							out << "\t\t\t\t"
							    << "writeTypeList( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
						}
					} else {
						out << "\t\t\t\t"
						    << "writeEntityList( stream, m_" << attirbuteVector[i].getName() << " );" << std::endl;
					}
				}
			} else {
				std::cout << "muh" << std::endl;
			}
		} else if (att.getParamterType() == eEntityAttributeParamterType::Simple) {
			out << "\t\t\t\t"
			    << "if( m_" << attirbuteVector[i].getName() << " == m_" << attirbuteVector[i].getName() << " ) { stream << m_" << attirbuteVector[i].getName() << "; }";
			out << std::endl;
			out << "\t\t\t\t"
			    << "else { stream << \"$\"; }" << std::endl;
		} else if (att.getParamterType() == eEntityAttributeParamterType::TypeNamed) {
			if (schema.hasEntity(att.type->toString())) {
				if (entity.hasQualifiedAttribute(attirbuteVector[i].getName())) {
					out << "\t\t\t\t"
					    << "if( m_" << attirbuteVector[i].getName() << ") { stream << \"#\" << m_" << attirbuteVector[i].getName() << "->getId(); } else { stream << \"*\"; }"
					    << std::endl;
				} else {
					out << "\t\t\t\t"
					    << "if( m_" << attirbuteVector[i].getName() << ") { stream << \"#\" << m_" << attirbuteVector[i].getName() << "->getId(); } else { stream << \"$\"; }"
					    << std::endl;
				}

			} else {
				if (entity.hasQualifiedAttribute(attirbuteVector[i].getName())) {
					out << "\t\t\t\t"
					    << "if( m_" << attirbuteVector[i].getName() << ") { m_" << attirbuteVector[i].getName() << "->getStepParameter( stream ); } else { stream << \"*\"; }"
					    << std::endl;
				} else {
					out << "\t\t\t\t"
					    << "if( m_" << attirbuteVector[i].getName() << ") { m_" << attirbuteVector[i].getName() << "->getStepParameter( stream ); } else { stream << \"$\"; }"
					    << std::endl;
				}
			}
		} else {
			if (entity.hasQualifiedAttribute(attirbuteVector[i].getName())) {
				out << "\t\t\t\t"
				    << "if( m_" << attirbuteVector[i].getName() << ") { m_" << attirbuteVector[i].getName() << "->getStepParameter( stream ); } else { stream << \"*\"; }"
				    << std::endl;
			} else {
				out << "\t\t\t\t"
				    << "if( m_" << attirbuteVector[i].getName() << ") { m_" << attirbuteVector[i].getName() << "->getStepParameter( stream ); } else { stream << \"$\"; }"
				    << std::endl;
			}
		}

		if (i != attirbuteVector.size() - 1) {
			out << "\t\t\t\t"
			    << "stream << \",\";" << std::endl;
		}
	}

	out << "\t\t\t\t"
	    << "stream << \");\";" << std::endl;

	out << "\t\t\t"
	    << "}" << std::endl;

	// getStepParameter
	out << "\t\t\t"
	    << "void " << entity.getName()
	    << "::getStepParameter( std::stringstream& stream, bool ) const { "
	       "stream << \"#\" << m_id; }"
	    << std::endl;

	// readStepData
	out << "\t\t\t"
	    << "void " << entity.getName()
	    << "::readStepData( std::vector<std::string>& args, const "
	       "std::map<int,shared_ptr<"
	    << schema.getName() << "Entity> >& map )" << std::endl;
	out << "\t\t\t"
	    << "{" << std::endl;
	out << "\t\t\t\t"
	    << "const int num_args = (int)args.size();" << std::endl;
	out << "\t\t\t\t"
	    << "if( num_args<" << attirbuteVector.size()
	    << " ){ std::stringstream strserr; strserr << \"Wrong parameter count "
	       "for entity "
	    << entity.getName() << ", expecting " << attirbuteVector.size()
	    << ", having \" << num_args << \". Object id: \" << getId() << "
	       "std::endl; throw "
	    << schema.getName() << "Exception( strserr.str().c_str() ); }" << std::endl;
	out << "\t\t\t\t"
	    << "#ifdef _DEBUG" << std::endl;
	out << "\t\t\t\t"
	    << "if( num_args<" << attirbuteVector.size() << " ){ std::cout << \"Wrong parameter count for entity " << entity.getName() << ", expecting " << attirbuteVector.size()
	    << ", having \" << num_args << \". Object id: \" << getId() << "
	       "std::endl; }"
	    << std::endl;
	out << "\t\t\t\t"
	    << "#endif" << std::endl;

	if (entity.getName() == "IfcTriangulatedFaceSet") {
		std::cout << "Test" << std::endl;
	}

	for (int i = 0; i < attirbuteVector.size(); i++) {
		const EntityAttribute &att = attirbuteVector[i];

		if (att.getParamterType() == eEntityAttributeParamterType::TypeNamed) {
			if (schema.hasType(att.type->toString())) {
				if (schema.isSelectType(att.type->toString())) {
					out << "\t\t\t\t"
					    << "m_" << att.getName() << " = " << att.type->toString() << "::readStepData( args[" << i << "], map );" << std::endl;
				} else {
					out << "\t\t\t\t"
					    << "m_" << att.getName() << " = " << att.type->toString() << "::readStepData( args[" << i << "] );" << std::endl;
				}
			} else {
				out << "\t\t\t\t"
				    << "readEntityReference( args[" << i << "], m_" << att.getName() << ", map);" << std::endl;
			}
		} else if (att.getParamterType() == eEntityAttributeParamterType::eGeneralizedType) {
			auto p = std::static_pointer_cast<EntityAttributeGeneralizedType>(att.type);

			if (p->containerType == eEntityAttributeContainerType::List || p->containerType == eEntityAttributeContainerType::Set
			    || p->containerType == eEntityAttributeContainerType::Array) {
				if (schema.hasType(p->elementType->toString())) {
					// is type a Real Type?
					if (schema.isRealType(p->elementType->toString())) {
						out << "\t\t\t\t"
						    << "readTypeOfRealList( args[" << i << "], m_" << att.getName() << " );" << std::endl;
					} else {
						if (schema.isSelectType(p->elementType->toString())) {
							out << "\t\t\t\t"
							    << "readSelectList( args[" << i << "], m_" << att.getName() << ", map );" << std::endl;
						} else {
							out << "\t\t\t\t"
							    << "readTypeList( args[" << i << "], m_" << att.getName() << " );" << std::endl;
						}
					}
				} else {
					if (p->elementType->getType() == eEntityAttributeParamterType::eGeneralizedType) {
						auto q = std::static_pointer_cast<EntityAttributeGeneralizedType>(p->elementType);

						if (schema.hasType(q->elementType->toString())) {
							const Type &type = schema.getTypeByName(q->elementType->toString());

							if (schema.isIntegerType(type.getName())) {
								out << "\t\t\t\t"
								    << "readTypeOfIntList2D( args[" << i << "], m_" << att.getName() << ");" << std::endl;
							} else if (type.getUnderlyingTypeName() == "REAL") {
								out << "\t\t\t\t"
								    << "readTypeOfRealList2D( args[" << i << "], m_" << att.getName() << ");" << std::endl;
							} else {
								out << "\t\t\t\t"
								    << "readEntityReferenceList2D( args[" << i << "], m_" << att.getName() << " , map);" << std::endl;
							}
						} else {
							out << "\t\t\t\t"
							    << "readEntityReferenceList2D( args[" << i << "], m_" << att.getName() << " , map);" << std::endl;
						}
					} else
						out << "\t\t\t\t"
						    << "readEntityReferenceList( args[" << i << "], m_" << att.getName() << " , map);" << std::endl;
				}
			}
		}
	}

	out << "\t\t\t"
	    << "}" << std::endl;

	// setInverseCounterparts
	out << "\t\t\t"
	    << "void " << entity.getName() << "::setInverseCounterparts(shared_ptr<" << schema.getName() << "Entity> ptr_self_entity)" << std::endl;
	out << "\t\t\t"
	    << "{" << std::endl;
	if (entity.hasSupertype()) {
		out << "\t\t\t\t" << entity.getSupertype() << "::setInverseCounterparts(ptr_self_entity);" << std::endl;
	}
	out << "\t\t\t"
	    << "}" << std::endl;

	// unlinkSelf
	out << "\t\t\t"
	    << "void " << entity.getName() << "::unlinkSelf()" << std::endl;
	out << "\t\t\t"
	    << "{" << std::endl;
	if (entity.hasSupertype()) {
		out << "\t\t\t\t" << entity.getSupertype() << "::unlinkSelf();" << std::endl;
	}
	out << "\t\t\t"
	    << "}" << std::endl;

	out << "\t"
	    << "}" << std::endl;
	out << "}" << std::endl;
}

OIP_NAMESPACE_OPENINFRAPLATFORM_EXPRESSBINDING_END
