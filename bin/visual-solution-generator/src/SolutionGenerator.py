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
rc_ext = "hlsl;jpg;jpeg;png;wav;mp3"

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
    
    project_type_guid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
    folder_type_guid = "2150E333-8FDC-42A3-9474-1A3956D46DE8"

    os.path.exists(vs_path) or os.makedirs(vs_path)
    with open(f"{vs_path + solution_name}.sln", 'w') as f:
        f.write("\n")
        f.write("Microsoft Visual Studio Solution File, Format Version 12.00\n")
        f.write("# Visual Studio Version 16\n")
        f.write("VisualStudioVersion = 16.0.28729.10\n")
        f.write("MinimumVisualStudioVersion = 10.0.40219.1\n")

        # Add projects
        for project in projects:
            # project['guid'] = newUUID()
            f.write(f"Project(\"{{{project_type_guid}}}\") = \"{project['name']}\", \"{project['folder'] + project['name']}.vcxproj\", \"{{{project['guid']}}}\"\n")

            # Add dependencies
            if project.get("dependencies"):
                f.write(f"\tProjectSection(ProjectDependencies) = postProject\n")
                for dependency in project['dependencies']:
                    f.write(f"\t\t{{{dependency}}} = {{{dependency}}}\n")
                f.write("\tEndProjectSection\n")
            f.write("EndProject\n")

        for folder in folders:
            f.write(f"Project(\"{{{folder_type_guid}}}\") = \"{folder['name']}\", \"{folder['name']}\", \"{folder['guid']}\"\n")
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
                f.write(f"\t\t{{{project['guid']}}} = {{{project['nested']}}}\n")
        for folder in folders:
            if folder.get('nested'):
                f.write(f"\t\t{{{folder['guid']}}} = {{{folder['nested']}}}\n")
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
    
    # Add configurations
    item_group = ET.SubElement(root, "ItemGroup", Label="ProjectConfigurations")
    for config in ["Debug", "Release"]:
        project_config = ET.SubElement(item_group, "ProjectConfiguration", Include=f"{config}|x64")
        ET.SubElement(project_config, "Configuration").text = config
        ET.SubElement(project_config, "Platform").text = "x64"

    # Globals
    property_group = ET.SubElement(root, "PropertyGroup", Label="Globals")
    ET.SubElement(property_group, "VCProjectVersion").text = project["vc_project_version"]
    ET.SubElement(property_group, "Keyword").text = "Win32Proj"
    ET.SubElement(property_group, "ProjectGuid").text = f"{{{project['guid']}}}"
    ET.SubElement(property_group, "RootNamespace").text = project["root_namespace"]
    ET.SubElement(property_group, "WindowsTargetPlatformVersion").text = project["windows_target_platform_version"]

    # Default props
    ET.SubElement(root, "Import", Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props")

    # Configurations
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        property_group_config = ET.SubElement(root, "PropertyGroup", Condition=condition, Label="Configuration")
        ET.SubElement(property_group_config, "ConfigurationType").text = config["type"]
        ET.SubElement(property_group_config, "UseDebugLibraries").text = config["use_debug_libraries"]
        ET.SubElement(property_group_config, "PlatformToolset").text = config["platform_toolset"]
        if config.get("whole_program_optimization"):
            ET.SubElement(property_group_config, "WholeProgramOptimization").text = config["whole_program_optimization"]
        ET.SubElement(property_group_config, "CharacterSet").text = config["character_set"]

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
        if config.get("out_dir"):
            ET.SubElement(property_group, "OutDir").text = config["out_dir"]
        if config.get("int_dir"):
            ET.SubElement(property_group, "IntDir").text = config["int_dir"]

    # Item Definition Groups
    for config in project["configuration"]:
        condition = f"'$(Configuration)|$(Platform)'=='{config['name']}'"
        item_definition_group = ET.SubElement(root, "ItemDefinitionGroup", Condition=condition)
        cl_compile = ET.SubElement(item_definition_group, "ClCompile")
        ET.SubElement(cl_compile, "WarningLevel").text = config["warning_level"]
        if config.get("function_level_linking"):
            ET.SubElement(cl_compile, "FunctionLevelLinking").text = config["function_level_linking"]
        if config.get("intrinsic_functions"):
            ET.SubElement(cl_compile, "IntrinsicFunctions").text = config["intrinsic_functions"]
        ET.SubElement(cl_compile, "SDLCheck").text = config["sdl_check"]
        ET.SubElement(cl_compile, "PreprocessorDefinitions").text = config["preprocessor_definitions"]
        ET.SubElement(cl_compile, "ConformanceMode").text = config["conformance_mode"]
        if config.get('precompiled_header'):
            ET.SubElement(cl_compile, "PrecompiledHeader").text = config["precompiled_header"]
        if config.get('precompiled_header_file'):
            ET.SubElement(cl_compile, "PrecompiledHeaderFile").text = config["precompiled_header_file"]
        if config.get('language_standard'):
            ET.SubElement(cl_compile, "LanguageStandard").text = config["language_standard"]
        if config.get('additional_include_directories'):
            ET.SubElement(cl_compile, "AdditionalIncludeDirectories").text = config["additional_include_directories"]

        # Link
        link = ET.SubElement(item_definition_group, "Link")
        if config.get("subsystem"):
            ET.SubElement(link, "SubSystem").text = config["subsystem"]
        if config.get("enable_comdat_folding"):
            ET.SubElement(link, "EnableCOMDATFolding").text = config["enable_comdat_folding"]
        if config.get("optimize_references"):
            ET.SubElement(link, "OptimizeReferences").text = config["optimize_references"]
        if config.get("generate_debug_information"):
            ET.SubElement(link, "GenerateDebugInformation").text = config["generate_debug_information"]
        if config.get("additional_library_directories"):
            ET.SubElement(link, "AdditionalLibraryDirectories").text = config["additional_library_directories"]
        if config.get("additional_dependencies"):
            ET.SubElement(link, "AdditionalDependencies").text = config["additional_dependencies"]
        

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

    if(not project.get('source_files')):
        project['source_files'] = []
    if(not project.get('header_files')):
        project['header_files'] = []
    if(not project.get('resource_files')):
        project['resource_files'] = []

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