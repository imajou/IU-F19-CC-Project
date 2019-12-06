; ModuleID = 'main'
source_filename = "main"

%Main = type {}

define void @main() {
entry:
  %Main = alloca %Main
  ret void
}

define internal void @Main(%Main*) {
entry:
  %this = alloca %Main*
  ret void
}
