set(ODB_SOURCES
  include/model/cppastnode.h
  include/model/cppentity.h
  include/model/cppenum.h
  include/model/cppfriendship.h
  include/model/cppfunction.h
  include/model/cppinheritance.h
  include/model/cppnamespace.h
  include/model/cppnamespacealias.h
  include/model/cpprelation.h
  include/model/cpptypedef.h
  include/model/cpprecord.h
  include/model/cppvariable.h
  include/model/cppheaderinclusion.h
  include/model/cppmacro.h
  include/model/cppmacroexpansion.h
  include/model/cppedge.h
  include/model/cppdoccomment.h
  include/model/cpptypedependency.h)

generate_odb_files("${ODB_SOURCES}")

add_odb_library(cppmodel ${ODB_CXX_SOURCES})
target_link_libraries(cppmodel model)

install_sql()
