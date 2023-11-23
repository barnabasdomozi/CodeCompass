#include <service/pythonservice.h>
#include <util/logutil.h>
#include <boost/python.hpp>
#include <string>
#include <odb/query.hxx>
#include <odb/database.hxx>
#include <model/file.h>
#include <model/file-odb.hxx>
#include <util/dbutil.h>
#include <util/odbtransaction.h>

namespace cc
{
namespace service
{
namespace language
{

namespace python = boost::python;

PythonServiceHandler::PythonServiceHandler(
  std::shared_ptr<odb::database> db_,
  std::shared_ptr<std::string> /*datadir_*/,
  const cc::webserver::ServerContext& context_)
    : _db(db_), _transaction(db_), _context(context_) 
    {
        // Init Python Interpreter
        std::string py_service_dir = _context.compassRoot + "/lib/serviceplugin/pyservice/";
        LOG(info) << "py_service_dir: " << py_service_dir;
        setenv("PYTHONPATH", py_service_dir.c_str(), 1);
        
        Py_Initialize();

        // Init PyService module
        try {
          m_py_module = python::import("service");
        }catch (const python::error_already_set&)
        {
          PyErr_Print();
        }
    }

void PythonServiceHandler::getFileTypes(
  std::vector<std::string>& return_) 
{
  LOG(info) << "[PYSERVICE] " << __func__ ;
  return_.push_back("PY");
  return_.push_back("Dir");

  return;
}

void PythonServiceHandler::getAstNodeInfo(
  AstNodeInfo& return_,
  const core::AstNodeId& astNodeId_) 
{
  LOG(info) << "[PYSERVICE] " << __func__;
  LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getAstNodeInfoByPosition(
  AstNodeInfo& return_,
  const core::FilePosition& fpos_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params fileId: " << std::stoull(fpos_.file);

_transaction([&, this](){
    model::File f;
    const service::core::FileId fileId = fpos_.file;

    if (!_db->find(std::stoull(fileId), f))
    {
      // TODO exception
      return;
    }

    LOG(info) << "path: " << f.path;

    // PyService
    try
    {
      python::object node = m_py_module.attr("getAstNodeInfoByPosition")(f.path, fpos_.pos.line, fpos_.pos.column);
      return_.id = python::extract<std::string>(node["id"]);
      return_.astNodeValue = python::extract<std::string>(node["value"]);
      return_.symbolType = python::extract<std::string>(node["type"]);
    }
    catch (const python::error_already_set&)
    {
      PyErr_Print();
    }
  });

  return;
}

void PythonServiceHandler::getSourceText(
  std::string& return_,
  const core::AstNodeId& astNodeId_) 
{
  LOG(info) << "[PYSERVICE] " << __func__;
  LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getDocumentation(
  std::string& return_,
  const core::AstNodeId& astNodeId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getProperties(
  std::map<std::string, std::string>& return_,
  const core::AstNodeId& astNodeId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getDiagramTypes(
  std::map<std::string, std::int32_t>& return_,
  const core::AstNodeId& astNodeId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getDiagram(
  std::string& return_,
  const core::AstNodeId& astNodeId_,
  const std::int32_t diagramId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getDiagramLegend(
  std::string& return_,
  const std::int32_t diagramId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getFileDiagramTypes(
  std::map<std::string, std::int32_t>& return_,
  const core::FileId& fileId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getFileDiagram(
  std::string& return_,
  const core::FileId& fileId_,
  const int32_t diagramId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getFileDiagramLegend(
  std::string& return_,
  const std::int32_t diagramId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getReferenceTypes(
  std::map<std::string, std::int32_t>& return_,
  const core::AstNodeId& astNodeId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  return_["Definition"]                = DEFINITION;
  return_["Usage"]                     = USAGE;
  return;
}

void PythonServiceHandler::getReferences(
  std::vector<AstNodeInfo>& return_,
  const core::AstNodeId& astNodeId_,
  const std::int32_t referenceId_,
  const std::vector<std::string>& tags_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  try
  {
    python::object refs = m_py_module.attr("getReferences")(astNodeId_, referenceId_);
    for (int i = 0; i < python::len(refs); i++)
    {
      python::object node = refs[i];
      AstNodeInfo nodeInfo;
      nodeInfo.id = python::extract<std::string>(node["id"]);
      nodeInfo.astNodeValue = python::extract<std::string>(node["value"]);
      nodeInfo.symbolType = python::extract<std::string>(node["type"]);
      nodeInfo.range.file = python::extract<std::string>(node["file_id"]);

      nodeInfo.range.range.startpos.line = python::extract<int>(node["start_line"]);
      nodeInfo.range.range.startpos.column = python::extract<int>(node["start_column"]);

      nodeInfo.range.range.endpos.line = python::extract<int>(node["end_line"]);
      nodeInfo.range.range.endpos.column = python::extract<int>(node["end_column"]);
      return_.push_back(nodeInfo);
    }
  }
  catch (const python::error_already_set&)
  {
    PyErr_Print();
  }
}

std::int32_t PythonServiceHandler::getReferenceCount(
  const core::AstNodeId& astNodeId_,
  const std::int32_t referenceId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  try
  {
    int c = python::extract<int>(m_py_module.attr("getReferenceCount")(astNodeId_, referenceId_));
    return c;
  }
  catch (const python::error_already_set&)
  {
    PyErr_Print();
  }
  return 0;
}

void PythonServiceHandler::getReferencesInFile(
  std::vector<AstNodeInfo>& return_,
  const core::AstNodeId& astNodeId_,
  const std::int32_t referenceId_,
  const core::FileId& fileId_,
  const std::vector<std::string>& tags_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
LOG(info) << "params astNodeId_: " << astNodeId_;
  return;
}

void PythonServiceHandler::getReferencesPage(
  std::vector<AstNodeInfo>& return_,
  const core::AstNodeId& astNodeId_,
  const std::int32_t referenceId_,
  const std::int32_t pageSize_,
  const std::int32_t pageNo_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getFileReferenceTypes(
  std::map<std::string, std::int32_t>& return_,
  const core::FileId& fileId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

void PythonServiceHandler::getFileReferences(
  std::vector<AstNodeInfo>& return_,
  const core::FileId& fileId_,
  const std::int32_t referenceId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

std::int32_t PythonServiceHandler::getFileReferenceCount(
  const core::FileId& fileId_,
  const std::int32_t referenceId_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return 0;
}

void PythonServiceHandler::getSyntaxHighlight(
  std::vector<SyntaxHighlight>& return_,
  const core::FileRange& range_) 
{
LOG(info) << "[PYSERVICE] " << __func__;
  return;
}

} // language
} // service
} // cc
