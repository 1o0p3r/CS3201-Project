List of Changes made




Class QueryValidator:
Add in new regexes
Modify method parseInput to make use of method isValidDeclarationRegex()
Modify method isValidEntity to make use method trim()
Added method trimPatternArgs(string str)
Optimize method isValidPattern, isValidEntity, isValidSuchThat
Added method IsValidDeclarationRegex
Added method isValidSuchThatRegex
Added method isValidPatternRegex
Added method isValidSelectIntialRgex
Modify the value of const string SUBSTRING
Modified isValidSuchThat and addSuchThatQueryElement methods to be able to add in arg1Ent and arg2Ent
Add method isValidSynDesignEntity

Class QueryElement:
Modify QueryElement and modify one of the constructors in QueryElement (such that clause) to be able to accommodate arg1Ent and arg2Ent
Modify QueryElement to have 2 getters i.e. getSuchThatArg1Ent() and getSuchThatArg2Ent()

Class RelationshipTable:
Modify RelationshipTable to have different valid design entities
