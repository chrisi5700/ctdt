file(REMOVE_RECURSE
  "libCTDT.a"
  "libCTDT.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/CTDT.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
