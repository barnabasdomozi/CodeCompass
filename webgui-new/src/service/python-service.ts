import thrift from 'thrift';
import { LanguageService, FilePosition, Position } from '@thrift-generated';
import { config } from './config';
import { toast } from 'react-toastify';

let client: LanguageService.Client | undefined;
export const createPythonClient = (workspace: string) => {
  if (!config) return;
  const connection = thrift.createXHRConnection(config.webserver_host, config.webserver_port, {
    transport: thrift.TBufferedTransport,
    protocol: thrift.TJSONProtocol,
    https: config.webserver_https,
    path: `${config.webserver_path}/${workspace}/PythonService`,
  });
  client = thrift.createXHRClient(LanguageService, connection);
  return client;
};

export const getPythonFileTypes = async () => {
  if (!client) {
    return [];
  }
  return await client.getFileTypes();
};

export const getPythonFileDiagramTypes = async (fileId: string) => {
  let resultMap = new Map<string, number>();
  if (!client) {
    return resultMap;
  }
  try {
    resultMap = await client.getFileDiagramTypes(fileId);
  } catch (e) {
    console.error(e);
    resultMap = new Map();
  }
  return resultMap;
};

export const getPythonFileDiagram = async (fileId: string, diagramId: number) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getFileDiagram(fileId, diagramId);
  } catch (e) {
    toast.error('Could not display diagram.');
    console.error(e);
    return '';
  }
};

export const getPythonFileDiagramLegend = async (diagramId: number) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getFileDiagramLegend(diagramId);
  } catch (e) {
    toast.error('Could not display diagram legend.');
    console.error(e);
    return '';
  }
};

export const getPythonDiagramTypes = async (astNodeId: string) => {
  let resultMap = new Map<string, number>();
  if (!client) {
    return resultMap;
  }
  try {
    resultMap = await client.getDiagramTypes(astNodeId);
  } catch (e) {
    console.error(e);
    resultMap = new Map();
  }
  return resultMap;
};

export const getPythonDiagram = async (astNodeId: string, diagramId: number) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getDiagram(astNodeId, diagramId);
  } catch (e) {
    toast.error('Could not display diagram.');
    console.error(e);
    return '';
  }
};

export const getPythonDiagramLegend = async (diagramId: number) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getDiagramLegend(diagramId);
  } catch (e) {
    toast.error('Could not display diagram legend.');
    console.error(e);
    return '';
  }
};

export const getPythonFileReferenceTypes = async (fileId: string) => {
  let resultMap = new Map<string, number>();
  if (!client) {
    return resultMap;
  }
  try {
    resultMap = await client.getFileReferenceTypes(fileId);
  } catch (e) {
    console.error(e);
    resultMap = new Map();
  }
  return resultMap;
};

export const getPythonFileReferences = async (fileId: string, referenceId: number) => {
  if (!client) {
    return [];
  }
  try {
    return await client.getFileReferences(fileId, referenceId);
  } catch (e) {
    console.error(e);
    return [];
  }
};

export const getPythonFileReferenceCount = async (fileId: string, referenceId: number) => {
  if (!client) {
    return 0;
  }
  try {
    return await client.getFileReferenceCount(fileId, referenceId);
  } catch (e) {
    console.error(e);
    return 0;
  }
};

export const getPythonReferenceTypes = async (astNodeId: string) => {
  let resultMap = new Map<string, number>();
  if (!client) {
    return resultMap;
  }
  try {
    resultMap = await client.getReferenceTypes(astNodeId);
  } catch (e) {
    console.error(e);
    resultMap = new Map();
  }
  return resultMap;
};

export const getPythonReferences = async (astNodeId: string, referenceId: number, tags: string[]) => {
  if (!client) {
    return [];
  }
  try {
    return await client.getReferences(astNodeId, referenceId, tags);
  } catch (e) {
    console.error(e);
    return [];
  }
};

export const getPythonReferenceCount = async (astNodeId: string, referenceId: number) => {
  if (!client) {
    return 0;
  }
  try {
    return await client.getReferenceCount(astNodeId, referenceId);
  } catch (e) {
    console.error(e);
    return 0;
  }
};

export const getPythonReferencesInFile = async (
  astNodeId: string,
  referenceId: number,
  fileId: string,
  tags: string[]
) => {
  if (!client) {
    return [];
  }
  try {
    return await client.getReferencesInFile(astNodeId, referenceId, fileId, tags);
  } catch (e) {
    console.error(e);
    return [];
  }
};

export const getPythonReferencesPage = async (
  astNodeId: string,
  referenceId: number,
  pageSize: number,
  pageNo: number
) => {
  if (!client) {
    return [];
  }
  try {
    return await client.getReferencesPage(astNodeId, referenceId, pageSize, pageNo);
  } catch (e) {
    console.error(e);
    return [];
  }
};

export const getPythonSourceText = async (astNodeId: string) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getSourceText(astNodeId);
  } catch (e) {
    console.error(e);
    return '';
  }
};

export const getPythonProperties = async (astNodeId: string) => {
  let resultMap = new Map<string, string>();
  if (!client) {
    return resultMap;
  }
  try {
    resultMap = await client.getProperties(astNodeId);
  } catch (e) {
    console.error(e);
    resultMap = new Map();
  }
  return resultMap;
};

export const getPythonDocumentation = async (astNodeId: string) => {
  if (!client) {
    return '';
  }
  try {
    return await client.getDocumentation(astNodeId);
  } catch (e) {
    toast.error('Could not get documentation about this AST node.');
    console.error(e);
    return '';
  }
};

export const getPythonAstNodeInfo = async (astNodeId: string) => {
  if (!client) {
    return;
  }
  try {
    return await client.getAstNodeInfo(astNodeId);
  } catch (e) {
    console.error(e);
    return;
  }
};

export const getPythonAstNodeInfoByPosition = async (fileId: string, line: number, column: number) => {
  if (!client) {
    return;
  }
  try {
    return await client.getAstNodeInfoByPosition(
      new FilePosition({
        file: fileId,
        pos: new Position({
          line,
          column,
        }),
      })
    );
  } catch (e) {
    return;
  }
};
