// ----------------------------------------------------------------
// fxDTSBrick
// The back-end for bricks. Handles bricks, their connections and 
// attributes, then sends them off to the oct tree for rendering. 
// ----------------------------------------------------------------

#include "game/fx/fxDTSBrick.h"
#include "console/consoleTypes.h"

StringTableEntry fxDTSBrick::sprayCanDivisionName[16];
S32              fxDTSBrick::sprayCanDivisionSlot[64];
ColorF           fxDTSBrick::colorIDTable[64];

// --------------------------------------------
// Functions to help with the spray can GUI
// --------------------------------------------

ConsoleFunction(setSprayCanDivision, void, 4, 4, "(int index, int slot, string name)")
{
   S32 index = dAtoi(argv[1]);
   S32 slot  = dAtoi(argv[2]);

   if (index < 16)
   {
      if (slot < 64)
      {
         StringTableEntry name = StringTable->insert(argv[3], false);
         fxDTSBrick::sprayCanDivisionName[index] = name;
         fxDTSBrick::sprayCanDivisionSlot[index] = slot;
         return;
      }
      Con::errorf("Error: setSprayCanDivision() - index out of range.");
   }
   else
   {
      Con::errorf("Error: setSprayCanDivision() - slot out of range.");
   }
   return;
}

ConsoleFunction(setColorTable, void, 3, 3, "(int colorID, string color)")
{
   S32 colorID = dAtoi(argv[1]);

   F32 r, g, b, a;

   if (colorID > 63)
   {
      Con::errorf("ERROR: setColorTable() - id out of range (%d)", colorID);
      return;
   }

   dSscanf(argv[2], "%f %f %f %f", &r, &g, &b, &a);

   fxDTSBrick::colorIDTable[colorID].red   = (r * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].green = (g * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].blue  = (b * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].alpha = (a * 255.0) / 255.0;
   return;
}

ConsoleFunction(getSprayCanDivisionName, const char*, 2, 2, "(int index)")
{
   S32 index = dAtoi(argv[1]);

   if (index < 16)
      return fxDTSBrick::sprayCanDivisionName[index];

   Con::errorf("Error: getSprayCanDivisionName() - index out of range.");
   return "";
}

ConsoleFunction(getSprayCanDivisionSlot, S32, 2, 2, "(int index)")
{
   S32 index = dAtoi(argv[1]);

   if (index < 16)
      return fxDTSBrick::sprayCanDivisionSlot[index];

   Con::errorf("Error: getSprayCanDivisionSlot() - index out of range.");
   return 0;
}

ConsoleFunction(getColorIDTable, const char*, 2, 2, "(int index)")
{
   char* buf = Con::getReturnBuffer(256);
   S32 index = dAtoi(argv[1]);
   F32 r, g, b, a;

   if (index <= 64)
   {
      ColorF color = fxDTSBrick::colorIDTable[index];
      r = color.red;
      g = color.green;
      b = color.blue;
      a = color.alpha;
   }
   else
   {
      Con::errorf("");
      Con::errorf("Error: getColorIDTable() - index \'%d\' out of range");
      Con::evaluatef("backtrace();");
      Con::errorf("");
      r = 0.0;
      g = 0.0;
      b = 0.0;
      a = 0.0;
   }
   dSprintf(buf, 256, "%f %f %f %f", r, g, b, a);
   return buf;
}