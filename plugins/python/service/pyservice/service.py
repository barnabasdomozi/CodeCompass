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

def loadName(x):
    file_path = str(x.module_path)
    start_column = x.get_definition_start_position()[1]
    end_column = x.get_definition_end_position()[1]

    s = f"{file_path}|{x.line}|{start_column}|{end_column}".encode("utf-8")
    id = str(sha1(s).hexdigest())

    if id not in names:
        names[id] = x
        names[id].file_id = str(fnvHash(file_path))

    return id

def dump(obj):
  for attr in dir(obj):
    print("obj.%s = %r" % (attr, getattr(obj, attr)))

def findRefs(id):
    if hasattr(names[id], 'refs'):
        return
    
    script = loadScript(str(names[id].module_path))
    name_refs = script.get_references(names[id].line, names[id].column)
    names[id].refs = list(map(lambda x : loadName(x), name_refs))
    names[id].defs = list(map(lambda x : loadName(x), names[id].goto(follow_imports=True)))
    names[id].uses = list(filter(lambda i : names[i].is_definition() == False, names[id].refs))

def fnvHash(str):
  hash = 14695981039346656037

  for c in str:
    hash ^= ord(c)
    hash *= 1099511628211

  # see: https://stackoverflow.com/questions/20766813/how-to-convert-signed-to-unsigned-integer-in-python
  return hash & 0xffffffffffffffff

def getAstNodeInfo(id):
    nodeInfo = {}

    if id in names:
        x = names[id]
        nodeInfo["id"] = id
        nodeInfo["value"] = x.get_line_code()
        nodeInfo["type"] = x.type
        nodeInfo["file_id"] = x.file_id
        nodeInfo["start_line"] = x.get_definition_start_position()[0]
        nodeInfo["start_column"] = x.get_definition_start_position()[1] + 1
        nodeInfo["end_line"] = x.get_definition_end_position()[0]
        nodeInfo["end_column"] = x.get_definition_end_position()[1] + 1

    return nodeInfo

def getAstNodeInfoByPosition(file_path, line, column):

    script = loadScript(file_path)
    column = column - 1

    for x in script.get_names(references = True):
        start_column = x.get_definition_start_position()[1]
        end_column = x.get_definition_end_position()[1]

        if x.line == line and column >= start_column and column <= end_column:
            return getAstNodeInfo(loadName(x))
            
def getReferenceCount(id, refId):
    if id not in names:
        return 0
    
    name = names[id]
    
    findRefs(id)
    
    if refId == 0:
        return len(name.defs)
    elif refId == 2:
        return len(name.uses)
    
    return 0

def getReferences(id, refId):
    if id not in names:
        return
    
    name = names[id]

    findRefs(id)

    if refId == 0:
        return list(map(lambda i : getAstNodeInfo(i), name.defs))
    elif refId == 2:
        return list(map(lambda i : getAstNodeInfo(i), name.uses))