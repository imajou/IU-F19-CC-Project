; ModuleID = 'main'
source_filename = "main"

%Animal = type { %Boolean*, %Real*, %Integer*, %Boolean*, %Integer* }
%Real = type { %Real* }
%Boolean = type { %Boolean* }
%Integer = type { %Integer* }
%Chief = type { %Employee*, %Boolean* }
%Employee = type { %Human*, %Boolean*, %Integer* }
%Human = type { %Animal*, %Integer*, %Boolean* }
%Student = type { %Human*, %Real*, %Integer* }

define void @main() {
entry:
  %Animal = alloca %Animal
  %Boolean = alloca %Boolean
  %Chief = alloca %Chief
  %Employee = alloca %Employee
  %Human = alloca %Human
  %Integer = alloca %Integer
  %Real = alloca %Real
  %Student = alloca %Student
  ret void
}

define internal void @Animal(%Animal*, %Integer, %Boolean, %Integer, %Real, %Boolean) {
entry:
  %this = alloca %Animal*
  %iq_p = alloca %Integer
  %alive_p = alloca %Boolean
  %age_p = alloca %Integer
  %weight_p = alloca %Real
  %sex_p = alloca %Boolean
  ret void
}

define internal void @Human(%Human*, %Integer, %Boolean, %Integer, %Real, %Boolean, %Boolean, %Integer) {
entry:
  %this = alloca %Human*
  %iq_p = alloca %Integer
  %alive_p = alloca %Boolean
  %age_p = alloca %Integer
  %weight_p = alloca %Real
  %sex_p = alloca %Boolean
  %knowLanguage_p = alloca %Boolean
  %status_p = alloca %Integer
  ret void
}

define internal void @Student(%Student*, %Integer, %Boolean, %Integer, %Real, %Boolean, %Boolean, %Integer, %Integer, %Real) {
entry:
  %this = alloca %Student*
  %iq_p = alloca %Integer
  %alive_p = alloca %Boolean
  %age_p = alloca %Integer
  %weight_p = alloca %Real
  %sex_p = alloca %Boolean
  %knowLanguage_p = alloca %Boolean
  %status_p = alloca %Integer
  %averageSleep_p = alloca %Integer
  %averagePoints_p = alloca %Real
  ret void
}

define internal void @Employee(%Employee*, %Integer, %Boolean, %Integer, %Real, %Boolean, %Boolean, %Integer, %Integer, %Boolean) {
entry:
  %this = alloca %Employee*
  %iq_p = alloca %Integer
  %alive_p = alloca %Boolean
  %age_p = alloca %Integer
  %weight_p = alloca %Real
  %sex_p = alloca %Boolean
  %knowLanguage_p = alloca %Boolean
  %status_p = alloca %Integer
  %salary_p = alloca %Integer
  %loveWork_p = alloca %Boolean
  ret void
}

define internal void @Chief(%Chief*, %Integer, %Boolean, %Integer, %Real, %Boolean, %Boolean, %Integer, %Integer, %Boolean, %Boolean) {
entry:
  %this = alloca %Chief*
  %iq_p = alloca %Integer
  %alive_p = alloca %Boolean
  %age_p = alloca %Integer
  %weight_p = alloca %Real
  %sex_p = alloca %Boolean
  %knowLanguage_p = alloca %Boolean
  %status_p = alloca %Integer
  %salary_p = alloca %Integer
  %loveWork_p = alloca %Boolean
  %goodChief_p = alloca %Boolean
  ret void
}
