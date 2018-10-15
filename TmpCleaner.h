//
// Created by Bruno Liberini on 15/10/18.
//

#ifndef TP_TMPCLEANER_H
#define TP_TMPCLEANER_H

class TmpCleaner {
  public:
    static void clean_tmp();
    ~TmpCleaner();
  private:
    TmpCleaner();
    TmpCleaner(TmpCleaner&);
};

#endif //TP_TMPCLEANER_H
