#ifndef _PRINTER_H
#define _PRINTER_H

class Printer {
 public:
  void print();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in printer.cpp.
extern Printer printer;

// Global vars defined as extern here,
//  but intialized in printer.cpp.
// Note: `const` global vars can be initialized directly in the header file
//  and without the need to use `extern`.
extern char PRINTER_NAME[];

#endif
