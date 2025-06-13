# Cplus-SFML-GAME
A Mario-style platform game developed in C++ using SFML.This code additionally uses the ImGui library for graphical user interface.
## Sujet
  At each level, the scenery gradually evolves from a primitive environment to a modern world.     
  The obstacles along the way gradually become industrial elements, symbolizing humanity's progress through agricultural development, but also the damage caused to the natural environment.    
## GamePlay
 The gameplay involves guiding a character through various levels, each with unique maps and monsters.     
 Players can collect corresponding items—corn—as rewards, and reaching the final level constitutes victory.    
## Design Pattern
 This game uses two design patterns: Factory pattern and Component pattern.    
 --In this design, Map serves as an abstract base class or interface that defines the general characteristics of map terrain.    
    Different types of terrain inherit from Map and implement their own specific behaviors.    
 --In component-based design, each large map is a composite object composed of multiple subcomponents .     
    Each terrain object can also contain multiple smaller elements, such as different texture objects or detail elements.    
