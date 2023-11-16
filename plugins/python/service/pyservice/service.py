import jedi
from hashlib import sha1

scripts = {}
names = {}

def loadScript(file_path):
    if file_path in scripts:
        return scripts[file_path]

    with open(file_path) as f:
        source = f.read()
        script = jedi.Script(source, path=file_path)
        scripts[file_path] = script
        return script

#def loadName()

def getAstNodeInfo(id):
    nodeInfo = {}

    if id in names:
        x = names[id]
        nodeInfo["id"] = id
        nodeInfo["value"] = x.get_line_code()
        nodeInfo["type"] = x.type

    return nodeInfo

def getAstNodeInfoByPosition(file_id, file_path, line, column):

    script = loadScript(file_path)
    column = column - 1

    for x in script.get_names(references = True):
        start_column = x.get_definition_start_position()[1]
        end_column = x.get_definition_end_position()[1]

        if x.line == line and column >= start_column and column <= end_column:
            s = f"{file_path}|{line}|{start_column}|{end_column}".encode("utf-8")
            id = str(sha1(s).hexdigest())

            if id not in names:
                names[id] = x
                names[id].fileId = file_id
                names[id].refs = script.get_references(x.line, x.column)

            return getAstNodeInfo(id)
            
def getReferenceCount(id, refId):
    name = names[id]

    if name is None:
        return 0
    
    if refId == 0:
        return len(list(filter(lambda x : x.is_definition() == True, name.refs)))
    elif refId == 2:
        return len(list(filter(lambda x : x.is_definition() == False, name.refs)))
    
    return 0

def getReferences(id, refId):
    if id not in names:
        return
    
    name = names[id]

    if refId == 0:
        print("getReferences Definitions")
        for x in list(filter(lambda x : x.is_definition() == True, name.refs)):
            print(x, "path:", x.module_path, "line:", x.line, "column:", x.column)
    elif refId == 2:
        print("getReferences Usage")
        for x in list(filter(lambda x : x.is_definition() == False, name.refs)):
            print(x, "path:", x.module_path, "line:", x.line, "column:", x.column)