import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import uuid
import os
import shutil

# paths
root_path = "../../"
ide_path = root_path + "ide/"
vs_path = ide_path + "vs/"

# filter extension
src_ext = "cpp;c"
h_ext = "h;hpp"
rc_ext = "hlsl;jpg;jpeg;png;wav;mp3;ico;dds;rc"

guids = {}

def newUUID():
    return str(uuid.uuid4()).upper()

def prettify_xml(elem):
    """Return a pretty-printed XML string for the Element."""
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")

def generate_sln(data):
    solution_name = data['solution_name']
    projects = data['projects']
    if(not data.get('folders')):
        data['folders'] = []
    folders = data['folders']
    
    type_guid = {"project": "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942",
                 "folder": "2150E333-8FDC-42A3-9474-1A3956D46DE8"}
    
    # fill guid array with projet and folder
    guids = {"projects": {},
             "folders": {}}
    for project in projects:
        project['guid'] = newUUID()
        guids['projects'][project['name']] = project['guid']
    for folder in folders:
        folder['guid'] = newUUID()
        guids['folders'][folder['name']] = folder['guid']

    os.path.exists(vs_path) or os.makedirs(vs_path)
    with open(f"{vs_path + solution_name}.sln", 'w') as f:
        f.write("\n")
        f.write("Microsoft Visual Studio Solution File, Format Version " + data['format_version'] + "\n")
        f.write("# Visual Studio Version " + data['version'] + "\n")
        f.write("VisualStudioVersion = " + data['version_full'] + "\n")
        f.write("MinimumVisualStudioVersion = " + data['minimum_version'] + "\n")

        # Add projects
        for project in projects:
            f.write(f"Project(\"{{{type_guid['project']}}}\") = \"{project['name']}\", \"{project['folder'] + project['name']}.vcxproj\", \"{{{project['guid']}}}\"\n")

            # Add dependencies
            if project.get("dependencies"):
                f.write(f"\tProjectSection(ProjectDependencies) = postProject\n")
                for dependency in project['dependencies']:
                    f.write(f"\t\t{{{guids['projects'][dependency]}}} = {{{guids['projects'][dependency]}}}\n")
                f.write("\tEndProjectSection\n")
            f.write("EndProject\n")

        for folder in folders:
            f.write(f"Project(\"{{{type_guid['folder']}}}\") = \"{folder['name']}\", \"{folder['name']}\", \"{folder['guid']}\"\n")
            f.write("EndProject\n")
        
        # Add global section
        f.write("Global\n")
        f.write("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n")
        f.write("\t\tDebug|x64 = Debug|x64\n")
        f.write("\t\tRelease|x64 = Release|x64\n")
        f.write("\tEndGlobalSection\n")

        # Add project configurations
        f.write("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n")
        for project in projects:
            f.write(f"\t\t{{{project['guid']}}}.Debug|x64.ActiveCfg = Debug|x64\n")
            f.write(f"\t\t{{{project['guid']}}}.Debug|x64.Build.0 = Debug|x64\n")
            f.write(f"\t\t{{{project['guid']}}}.Release|x64.ActiveCfg = Release|x64\n")
            f.write(f"\t\t{{{project['guid']}}}.Release|x64.Build.0 = Release|x64\n")
        f.write("\tEndGlobalSection\n")

        f.write("\tGlobalSection(SolutionProperties) = preSolution\n")
        f.write("\t\tHideSolutionNode = FALSE\n")
        f.write("\tEndGlobalSection\n")

        # Add nested projects
        f.write("\tGlobalSection(NestedProjects) = preSolution\n")
        for project in projects:
            if project.get('nested'):
                f.write(f"\t\t{{{project['guid']}}} = {{{guids['folders'][project['nested']]}}}\n")
        for folder in folders:
            if folder.get('nested'):
                f.write(f"\t\t{{{folder['guid']}}} = {{{guids['folders'][folder['nested']]}}}\n")
        f.write("\tEndGlobalSection\n")

        f.write("\tGlobalSection(ExtensibilityGlobals) = postSolution\n")
        f.write(f"\t\tSolutionGuid = {{{newUUID()}}}\n")
        f.write("\tEndGlobalSection\n")

        f.write("EndGlobal\n")

def generate_filters(project):
    root = ET.Element("Project", ToolsVersion="4.0", xmlns="http://schemas.microsoft.com/developer/msbuild/2003")
    
    # Function to add files to the ItemGroup
    def add_files_to_item_group(item_group, tag, files, filter_name):
        for file in files:
            file_element = ET.SubElement(item_group, tag, Include=file)
            filter_element = ET.SubElement(file_element, "Filter")
            filter_element.text = filter_name
    
    # Create filters
    item_group = ET.SubElement(root, "ItemGroup")  

    filters = [
        ("Source Files", src_ext),
        ("Header Files", h_ext),
        ("Resource Files", rc_ext),
        # ("Other Files", ""),
    ]      

    for filter_name, extensions in filters:
        filter_element = ET.SubElement(item_group, "Filter", Include=filter_name)
        ET.SubElement(filter_element, "UniqueIdentifier").text = "{" + newUUID() + "}"
        ET.SubElement(filter_element, "Extensions").text = extensions
    
    # Add files to their respective filters
    item_group = ET.SubElement(root, "ItemGroup")
    add_files_to_item_group(item_group, "ClCompile", project['source_files'], "Source Files")
    add_files_to_item_group(item_group, "ClInclude", project['header_files'], "Header Files")
    add_files_to_item_group(item_group, "None", project['resource_files'], "Resource Files")
    # add_files_to_item_group(item_group, "None", project['other_files'], "Other Files")
    
    # Prettify and write the XML
    pretty_xml_as_string = prettify_xml(root)
    
    path = vs_path + project['folder'] + project['name'] + ".vcxproj.filters"
    dir_name = os.path.dirname(path)
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

    with open(f"{path}", "w") as f:
        f.write(pretty_xml_as_string)

def generate_vcxproj(project):
    root = ET.Element("Project", ToolsVersion="4.0", xmlns="http://schemas.microsoft.com/developer/msbuild/2003")
    
    def add_text_element(parent, tag, text):
        if text is None:
            return
        element = ET.SubElement(parent, tag)
        element.text = text
        return element

    # Add configurations
    item_group = ET.SubElement(root, "ItemGroup", Label="ProjectConfigurations")
    for config in ["Debug", "Release"]:
        project_config = ET.SubElement(item_group, "ProjectConfiguration", Include=f"{config}|x64")
        ET.SubElement(project_config, "Configuration").text = config
        ET.SubElement(project_config, "Platform").text = "x64"

    # Globals
    property_group = ET.SubElement(root, "PropertyGroup", Label="Globals")
    add_text_element(property_group, "VCProjectVersion", project.get("vc_project_version"))
    add_text_element(property_group, "Keyword", "Win32Proj")
    add_text_element(property_group, "ProjectGuid", f"{{{project['guid']}}})")
    add_text_element(property_group, "RootNamespace", project.get("root_namespace"))
    add_text_element(property_group, "WindowsTargetPlatformVersion", project.get("windows_target_platform_version"))

    # Default props
    ET.SubElement(root, "Import", Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props")

    # Configurations
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        property_group_config = ET.SubElement(root, "PropertyGroup", Condition=condition, Label="Configuration")
        add_text_element(property_group_config, "ConfigurationType", config.get("type"))
        add_text_element(property_group_config, "UseDebugLibraries", config.get("use_debug_libraries"))
        add_text_element(property_group_config, "PlatformToolset", config.get("platform_toolset"))
        add_text_element(property_group_config, "WholeProgramOptimization", config.get("whole_program_optimization"))
        add_text_element(property_group_config, "CharacterSet", config.get("character_set"))

    # Cpp props
    ET.SubElement(root, "Import", Project="$(VCTargetsPath)\\Microsoft.Cpp.props")
    ET.SubElement(root, "ImportGroup", Label="ExtensionSettings")
    ET.SubElement(root, "ImportGroup", Label="Shared")

    # Property Sheets
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        import_group = ET.SubElement(root, "ImportGroup", Label="PropertySheets", Condition=condition)
        ET.SubElement(import_group, "Import", Project="$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props", Condition="exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')", Label="LocalAppDataPlatform")
    
    # User Macros
    ET.SubElement(root, "PropertyGroup", Label="UserMacros")

    # Output directory
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        property_group = ET.SubElement(root, "PropertyGroup", Condition=condition)
        add_text_element(property_group, "OutDir", config.get("out_dir"))
        add_text_element(property_group, "IntDir", config.get("int_dir"))

    # Item Definition Groups
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        item_definition_group = ET.SubElement(root, "ItemDefinitionGroup", Condition=condition)
        cl_compile = ET.SubElement(item_definition_group, "ClCompile")
        add_text_element(cl_compile, "WarningLevel", config.get("warning_level"))
        add_text_element(cl_compile, "FunctionLevelLinking", config.get("function_level_linking"))
        add_text_element(cl_compile, "IntrinsicFunctions", config.get("intrinsic_functions"))
        add_text_element(cl_compile, "SDLCheck", config.get("sdl_check"))
        add_text_element(cl_compile, "PreprocessorDefinitions", config.get("preprocessor_definitions"))
        add_text_element(cl_compile, "ConformanceMode", config.get("conformance_mode"))
        add_text_element(cl_compile, "PrecompiledHeader", config.get("precompiled_header"))
        add_text_element(cl_compile, "PrecompiledHeaderFile", config.get("precompiled_header_file"))
        add_text_element(cl_compile, "LanguageStandard", config.get("language_standard"))
        add_text_element(cl_compile, "AdditionalIncludeDirectories", config.get("additional_include_directories"))

        # Link
        link = ET.SubElement(item_definition_group, "Link")
        add_text_element(link, "SubSystem", config.get("subsystem"))
        add_text_element(link, "EnableCOMDATFolding", config.get("enable_comdat_folding"))
        add_text_element(link, "OptimizeReferences", config.get("optimize_references"))
        add_text_element(link, "GenerateDebugInformation", config.get("generate_debug_information"))
        add_text_element(link, "AdditionalLibraryDirectories", config.get("additional_library_directories"))
        add_text_element(link, "AdditionalDependencies", config.get("additional_dependencies"))
        

    # Item Groups for source and header files
    item_group_clinclude = ET.SubElement(root, "ItemGroup")
    for header in project["header_files"]:
        ET.SubElement(item_group_clinclude, "ClInclude", Include=header)

    item_group_none = ET.SubElement(root, "ItemGroup")
    for file in project["resource_files"]:
        ET.SubElement(item_group_none, "None", Include=file)
    
    item_group_clcompile = ET.SubElement(root, "ItemGroup")
    for source in project["source_files"]:
        cl_compile = ET.SubElement(item_group_clcompile, "ClCompile", Include=source)
        if source.endswith("pch.cpp"):
            ET.SubElement(cl_compile, "PrecompiledHeader", Condition="'$(Configuration)|$(Platform)'=='Debug|x64'").text = "Create"
            ET.SubElement(cl_compile, "PrecompiledHeader", Condition="'$(Configuration)|$(Platform)'=='Release|x64'").text = "Create"
    
    # Cpp targets and ExtensionTargets
    ET.SubElement(root, "Import", Project="$(VCTargetsPath)\\Microsoft.Cpp.targets")
    ET.SubElement(root, "ImportGroup", Label="ExtensionTargets")

    # Prettify and write the XML
    pretty_xml_as_string = prettify_xml(root)

    path = vs_path + project['folder'] + project['name'] + ".vcxproj"
    dir_name = os.path.dirname(path)
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

    with open(f"{path}", "w") as f:
        f.write(pretty_xml_as_string)

def populate_include_files(project):
    src_path = root_path + "src/"

    src_ext_split = src_ext.split(';')
    h_ext_split = h_ext.split(';')
    rc_ext_split = rc_ext.split(';')

    filters = ["source_files", "header_files", "resource_files", "other_files"]
    for filter in filters:
        if(not project.get(filter)):
            project[filter] = []

    if os.path.exists(src_path + project['folder']):
        for dirpath, dirnames, filenames in os.walk(src_path + project['folder']):
            for filename in filenames:
                ext = filename.split('.')[-1]
                relative_path = os.path.relpath(dirpath + '/' + filename, start=vs_path + project['folder'])
                if ext in src_ext_split:
                    project['source_files'].append(relative_path)
                if ext in h_ext_split:
                    project['header_files'].append(relative_path)
                if ext in rc_ext_split:
                    project['resource_files'].append(relative_path)
                if ext not in (src_ext_split + h_ext_split + rc_ext_split):
                    project['other_files'].append(relative_path)
    else:
        os.makedirs(src_path + project['folder'])

def generate_solution(data):
    
    # Remove the ide folder
    if os.path.exists(ide_path):
        try:
            shutil.rmtree(ide_path)
        except Exception as e:
            print(f"An error occurred while deleting the ide folder: {e}")

    # Generate the solution
    generate_sln(data)
    file_path = os.path.realpath(vs_path + data['solution_name'])
    print(file_path + ".sln generated successfully!")

    # Generate the projects
    for project in data['projects']:
        populate_include_files(project)
        generate_vcxproj(project)
        generate_filters(project)
        
        file_path = os.path.realpath(vs_path + project['name'])
        print(file_path + ".vcxproj generated successfully!")
        print(file_path + ".vcxproj.filters generated successfully!")
    
    # open the file explorer
    app_path = os.path.realpath(vs_path)

    # Specify the path to the __pycache__ directory
    pycache_path = './src/__pycache__'
    if os.path.exists(pycache_path):
        shutil.rmtree(pycache_path)

    return app_path