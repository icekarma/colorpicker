#include "pch.h"

#include "ChannelInformation.h"
#include "Utils.h"

namespace {

    //
    // Private constants
    //

    CRect const BadRect { { -65536, -65536 }, SIZE { -1, -1 } };

    std::unordered_map<UINT, wchar_t const*> const WindowsMessageNames {
        { WM_NULL,                           L"WM_NULL"                           },
        { WM_CREATE,                         L"WM_CREATE"                         },
        { WM_DESTROY,                        L"WM_DESTROY"                        },
        { WM_MOVE,                           L"WM_MOVE"                           },
        { WM_SIZE,                           L"WM_SIZE"                           },
        { WM_ACTIVATE,                       L"WM_ACTIVATE"                       },
        { WM_SETFOCUS,                       L"WM_SETFOCUS"                       },
        { WM_KILLFOCUS,                      L"WM_KILLFOCUS"                      },
        { WM_ENABLE,                         L"WM_ENABLE"                         },
        { WM_SETREDRAW,                      L"WM_SETREDRAW"                      },
        { WM_SETTEXT,                        L"WM_SETTEXT"                        },
        { WM_GETTEXT,                        L"WM_GETTEXT"                        },
        { WM_GETTEXTLENGTH,                  L"WM_GETTEXTLENGTH"                  },
        { WM_PAINT,                          L"WM_PAINT"                          },
        { WM_CLOSE,                          L"WM_CLOSE"                          },
        { WM_QUERYENDSESSION,                L"WM_QUERYENDSESSION"                },
        { WM_QUERYOPEN,                      L"WM_QUERYOPEN"                      },
        { WM_ENDSESSION,                     L"WM_ENDSESSION"                     },
        { WM_QUIT,                           L"WM_QUIT"                           },
        { WM_ERASEBKGND,                     L"WM_ERASEBKGND"                     },
        { WM_SYSCOLORCHANGE,                 L"WM_SYSCOLORCHANGE"                 },
        { WM_SHOWWINDOW,                     L"WM_SHOWWINDOW"                     },
        { WM_WININICHANGE,                   L"WM_WININICHANGE"                   },
        { WM_DEVMODECHANGE,                  L"WM_DEVMODECHANGE"                  },
        { WM_ACTIVATEAPP,                    L"WM_ACTIVATEAPP"                    },
        { WM_FONTCHANGE,                     L"WM_FONTCHANGE"                     },
        { WM_TIMECHANGE,                     L"WM_TIMECHANGE"                     },
        { WM_CANCELMODE,                     L"WM_CANCELMODE"                     },
        { WM_SETCURSOR,                      L"WM_SETCURSOR"                      },
        { WM_MOUSEACTIVATE,                  L"WM_MOUSEACTIVATE"                  },
        { WM_CHILDACTIVATE,                  L"WM_CHILDACTIVATE"                  },
        { WM_QUEUESYNC,                      L"WM_QUEUESYNC"                      },
        { WM_GETMINMAXINFO,                  L"WM_GETMINMAXINFO"                  },
        { WM_PAINTICON,                      L"WM_PAINTICON"                      },
        { WM_ICONERASEBKGND,                 L"WM_ICONERASEBKGND"                 },
        { WM_NEXTDLGCTL,                     L"WM_NEXTDLGCTL"                     },
        { WM_SPOOLERSTATUS,                  L"WM_SPOOLERSTATUS"                  },
        { WM_DRAWITEM,                       L"WM_DRAWITEM"                       },
        { WM_MEASUREITEM,                    L"WM_MEASUREITEM"                    },
        { WM_DELETEITEM,                     L"WM_DELETEITEM"                     },
        { WM_VKEYTOITEM,                     L"WM_VKEYTOITEM"                     },
        { WM_CHARTOITEM,                     L"WM_CHARTOITEM"                     },
        { WM_SETFONT,                        L"WM_SETFONT"                        },
        { WM_GETFONT,                        L"WM_GETFONT"                        },
        { WM_SETHOTKEY,                      L"WM_SETHOTKEY"                      },
        { WM_GETHOTKEY,                      L"WM_GETHOTKEY"                      },
        { WM_QUERYDRAGICON,                  L"WM_QUERYDRAGICON"                  },
        { WM_COMPAREITEM,                    L"WM_COMPAREITEM"                    },
        { WM_GETOBJECT,                      L"WM_GETOBJECT"                      },
        { WM_COMPACTING,                     L"WM_COMPACTING"                     },
        { WM_COMMNOTIFY,                     L"WM_COMMNOTIFY"                     },
        { WM_WINDOWPOSCHANGING,              L"WM_WINDOWPOSCHANGING"              },
        { WM_WINDOWPOSCHANGED,               L"WM_WINDOWPOSCHANGED"               },
        { WM_POWER,                          L"WM_POWER"                          },
        { WM_COPYDATA,                       L"WM_COPYDATA"                       },
        { WM_CANCELJOURNAL,                  L"WM_CANCELJOURNAL"                  },
        { WM_NOTIFY,                         L"WM_NOTIFY"                         },
        { WM_INPUTLANGCHANGEREQUEST,         L"WM_INPUTLANGCHANGEREQUEST"         },
        { WM_INPUTLANGCHANGE,                L"WM_INPUTLANGCHANGE"                },
        { WM_TCARD,                          L"WM_TCARD"                          },
        { WM_HELP,                           L"WM_HELP"                           },
        { WM_USERCHANGED,                    L"WM_USERCHANGED"                    },
        { WM_NOTIFYFORMAT,                   L"WM_NOTIFYFORMAT"                   },
        { WM_CONTEXTMENU,                    L"WM_CONTEXTMENU"                    },
        { WM_STYLECHANGING,                  L"WM_STYLECHANGING"                  },
        { WM_STYLECHANGED,                   L"WM_STYLECHANGED"                   },
        { WM_DISPLAYCHANGE,                  L"WM_DISPLAYCHANGE"                  },
        { WM_GETICON,                        L"WM_GETICON"                        },
        { WM_SETICON,                        L"WM_SETICON"                        },
        { WM_NCCREATE,                       L"WM_NCCREATE"                       },
        { WM_NCDESTROY,                      L"WM_NCDESTROY"                      },
        { WM_NCCALCSIZE,                     L"WM_NCCALCSIZE"                     },
        { WM_NCHITTEST,                      L"WM_NCHITTEST"                      },
        { WM_NCPAINT,                        L"WM_NCPAINT"                        },
        { WM_NCACTIVATE,                     L"WM_NCACTIVATE"                     },
        { WM_GETDLGCODE,                     L"WM_GETDLGCODE"                     },
        { WM_SYNCPAINT,                      L"WM_SYNCPAINT"                      },
        { WM_NCMOUSEMOVE,                    L"WM_NCMOUSEMOVE"                    },
        { WM_NCLBUTTONDOWN,                  L"WM_NCLBUTTONDOWN"                  },
        { WM_NCLBUTTONUP,                    L"WM_NCLBUTTONUP"                    },
        { WM_NCLBUTTONDBLCLK,                L"WM_NCLBUTTONDBLCLK"                },
        { WM_NCRBUTTONDOWN,                  L"WM_NCRBUTTONDOWN"                  },
        { WM_NCRBUTTONUP,                    L"WM_NCRBUTTONUP"                    },
        { WM_NCRBUTTONDBLCLK,                L"WM_NCRBUTTONDBLCLK"                },
        { WM_NCMBUTTONDOWN,                  L"WM_NCMBUTTONDOWN"                  },
        { WM_NCMBUTTONUP,                    L"WM_NCMBUTTONUP"                    },
        { WM_NCMBUTTONDBLCLK,                L"WM_NCMBUTTONDBLCLK"                },
        { WM_NCXBUTTONDOWN,                  L"WM_NCXBUTTONDOWN"                  },
        { WM_NCXBUTTONUP,                    L"WM_NCXBUTTONUP"                    },
        { WM_NCXBUTTONDBLCLK,                L"WM_NCXBUTTONDBLCLK"                },
        { WM_INPUT_DEVICE_CHANGE,            L"WM_INPUT_DEVICE_CHANGE"            },
        { WM_INPUT,                          L"WM_INPUT"                          },
        { WM_KEYDOWN,                        L"WM_KEYDOWN"                        },
        { WM_KEYUP,                          L"WM_KEYUP"                          },
        { WM_CHAR,                           L"WM_CHAR"                           },
        { WM_DEADCHAR,                       L"WM_DEADCHAR"                       },
        { WM_SYSKEYDOWN,                     L"WM_SYSKEYDOWN"                     },
        { WM_SYSKEYUP,                       L"WM_SYSKEYUP"                       },
        { WM_SYSCHAR,                        L"WM_SYSCHAR"                        },
        { WM_SYSDEADCHAR,                    L"WM_SYSDEADCHAR"                    },
        { WM_UNICHAR,                        L"WM_UNICHAR"                        },
        { WM_IME_STARTCOMPOSITION,           L"WM_IME_STARTCOMPOSITION"           },
        { WM_IME_ENDCOMPOSITION,             L"WM_IME_ENDCOMPOSITION"             },
        { WM_IME_COMPOSITION,                L"WM_IME_COMPOSITION"                },
        { WM_INITDIALOG,                     L"WM_INITDIALOG"                     },
        { WM_COMMAND,                        L"WM_COMMAND"                        },
        { WM_SYSCOMMAND,                     L"WM_SYSCOMMAND"                     },
        { WM_TIMER,                          L"WM_TIMER"                          },
        { WM_HSCROLL,                        L"WM_HSCROLL"                        },
        { WM_VSCROLL,                        L"WM_VSCROLL"                        },
        { WM_INITMENU,                       L"WM_INITMENU"                       },
        { WM_INITMENUPOPUP,                  L"WM_INITMENUPOPUP"                  },
        { WM_GESTURE,                        L"WM_GESTURE"                        },
        { WM_GESTURENOTIFY,                  L"WM_GESTURENOTIFY"                  },
        { WM_MENUSELECT,                     L"WM_MENUSELECT"                     },
        { WM_MENUCHAR,                       L"WM_MENUCHAR"                       },
        { WM_ENTERIDLE,                      L"WM_ENTERIDLE"                      },
        { WM_MENURBUTTONUP,                  L"WM_MENURBUTTONUP"                  },
        { WM_MENUDRAG,                       L"WM_MENUDRAG"                       },
        { WM_MENUGETOBJECT,                  L"WM_MENUGETOBJECT"                  },
        { WM_UNINITMENUPOPUP,                L"WM_UNINITMENUPOPUP"                },
        { WM_MENUCOMMAND,                    L"WM_MENUCOMMAND"                    },
        { WM_CHANGEUISTATE,                  L"WM_CHANGEUISTATE"                  },
        { WM_UPDATEUISTATE,                  L"WM_UPDATEUISTATE"                  },
        { WM_QUERYUISTATE,                   L"WM_QUERYUISTATE"                   },
        { WM_CTLCOLORMSGBOX,                 L"WM_CTLCOLORMSGBOX"                 },
        { WM_CTLCOLOREDIT,                   L"WM_CTLCOLOREDIT"                   },
        { WM_CTLCOLORLISTBOX,                L"WM_CTLCOLORLISTBOX"                },
        { WM_CTLCOLORBTN,                    L"WM_CTLCOLORBTN"                    },
        { WM_CTLCOLORDLG,                    L"WM_CTLCOLORDLG"                    },
        { WM_CTLCOLORSCROLLBAR,              L"WM_CTLCOLORSCROLLBAR"              },
        { WM_CTLCOLORSTATIC,                 L"WM_CTLCOLORSTATIC"                 },
        { MN_GETHMENU,                       L"MN_GETHMENU"                       },
        { WM_MOUSEMOVE,                      L"WM_MOUSEMOVE"                      },
        { WM_LBUTTONDOWN,                    L"WM_LBUTTONDOWN"                    },
        { WM_LBUTTONUP,                      L"WM_LBUTTONUP"                      },
        { WM_LBUTTONDBLCLK,                  L"WM_LBUTTONDBLCLK"                  },
        { WM_RBUTTONDOWN,                    L"WM_RBUTTONDOWN"                    },
        { WM_RBUTTONUP,                      L"WM_RBUTTONUP"                      },
        { WM_RBUTTONDBLCLK,                  L"WM_RBUTTONDBLCLK"                  },
        { WM_MBUTTONDOWN,                    L"WM_MBUTTONDOWN"                    },
        { WM_MBUTTONUP,                      L"WM_MBUTTONUP"                      },
        { WM_MBUTTONDBLCLK,                  L"WM_MBUTTONDBLCLK"                  },
        { WM_MOUSEWHEEL,                     L"WM_MOUSEWHEEL"                     },
        { WM_XBUTTONDOWN,                    L"WM_XBUTTONDOWN"                    },
        { WM_XBUTTONUP,                      L"WM_XBUTTONUP"                      },
        { WM_XBUTTONDBLCLK,                  L"WM_XBUTTONDBLCLK"                  },
        { WM_MOUSEHWHEEL,                    L"WM_MOUSEHWHEEL"                    },
        { WM_PARENTNOTIFY,                   L"WM_PARENTNOTIFY"                   },
        { WM_ENTERMENULOOP,                  L"WM_ENTERMENULOOP"                  },
        { WM_EXITMENULOOP,                   L"WM_EXITMENULOOP"                   },
        { WM_NEXTMENU,                       L"WM_NEXTMENU"                       },
        { WM_SIZING,                         L"WM_SIZING"                         },
        { WM_CAPTURECHANGED,                 L"WM_CAPTURECHANGED"                 },
        { WM_MOVING,                         L"WM_MOVING"                         },
        { WM_POWERBROADCAST,                 L"WM_POWERBROADCAST"                 },
        { WM_DEVICECHANGE,                   L"WM_DEVICECHANGE"                   },
        { WM_MDICREATE,                      L"WM_MDICREATE"                      },
        { WM_MDIDESTROY,                     L"WM_MDIDESTROY"                     },
        { WM_MDIACTIVATE,                    L"WM_MDIACTIVATE"                    },
        { WM_MDIRESTORE,                     L"WM_MDIRESTORE"                     },
        { WM_MDINEXT,                        L"WM_MDINEXT"                        },
        { WM_MDIMAXIMIZE,                    L"WM_MDIMAXIMIZE"                    },
        { WM_MDITILE,                        L"WM_MDITILE"                        },
        { WM_MDICASCADE,                     L"WM_MDICASCADE"                     },
        { WM_MDIICONARRANGE,                 L"WM_MDIICONARRANGE"                 },
        { WM_MDIGETACTIVE,                   L"WM_MDIGETACTIVE"                   },
        { WM_MDISETMENU,                     L"WM_MDISETMENU"                     },
        { WM_ENTERSIZEMOVE,                  L"WM_ENTERSIZEMOVE"                  },
        { WM_EXITSIZEMOVE,                   L"WM_EXITSIZEMOVE"                   },
        { WM_DROPFILES,                      L"WM_DROPFILES"                      },
        { WM_MDIREFRESHMENU,                 L"WM_MDIREFRESHMENU"                 },
        { WM_POINTERDEVICECHANGE,            L"WM_POINTERDEVICECHANGE"            },
        { WM_POINTERDEVICEINRANGE,           L"WM_POINTERDEVICEINRANGE"           },
        { WM_POINTERDEVICEOUTOFRANGE,        L"WM_POINTERDEVICEOUTOFRANGE"        },
        { WM_TOUCH,                          L"WM_TOUCH"                          },
        { WM_NCPOINTERUPDATE,                L"WM_NCPOINTERUPDATE"                },
        { WM_NCPOINTERDOWN,                  L"WM_NCPOINTERDOWN"                  },
        { WM_NCPOINTERUP,                    L"WM_NCPOINTERUP"                    },
        { WM_POINTERUPDATE,                  L"WM_POINTERUPDATE"                  },
        { WM_POINTERDOWN,                    L"WM_POINTERDOWN"                    },
        { WM_POINTERUP,                      L"WM_POINTERUP"                      },
        { WM_POINTERENTER,                   L"WM_POINTERENTER"                   },
        { WM_POINTERLEAVE,                   L"WM_POINTERLEAVE"                   },
        { WM_POINTERACTIVATE,                L"WM_POINTERACTIVATE"                },
        { WM_POINTERCAPTURECHANGED,          L"WM_POINTERCAPTURECHANGED"          },
        { WM_TOUCHHITTESTING,                L"WM_TOUCHHITTESTING"                },
        { WM_POINTERWHEEL,                   L"WM_POINTERWHEEL"                   },
        { WM_POINTERHWHEEL,                  L"WM_POINTERHWHEEL"                  },
        { DM_POINTERHITTEST,                 L"DM_POINTERHITTEST"                 },
        { WM_POINTERROUTEDTO,                L"WM_POINTERROUTEDTO"                },
        { WM_POINTERROUTEDAWAY,              L"WM_POINTERROUTEDAWAY"              },
        { WM_POINTERROUTEDRELEASED,          L"WM_POINTERROUTEDRELEASED"          },
        { WM_IME_SETCONTEXT,                 L"WM_IME_SETCONTEXT"                 },
        { WM_IME_NOTIFY,                     L"WM_IME_NOTIFY"                     },
        { WM_IME_CONTROL,                    L"WM_IME_CONTROL"                    },
        { WM_IME_COMPOSITIONFULL,            L"WM_IME_COMPOSITIONFULL"            },
        { WM_IME_SELECT,                     L"WM_IME_SELECT"                     },
        { WM_IME_CHAR,                       L"WM_IME_CHAR"                       },
        { WM_IME_REQUEST,                    L"WM_IME_REQUEST"                    },
        { WM_IME_KEYDOWN,                    L"WM_IME_KEYDOWN"                    },
        { WM_IME_KEYUP,                      L"WM_IME_KEYUP"                      },
        { WM_MOUSEHOVER,                     L"WM_MOUSEHOVER"                     },
        { WM_MOUSELEAVE,                     L"WM_MOUSELEAVE"                     },
        { WM_NCMOUSEHOVER,                   L"WM_NCMOUSEHOVER"                   },
        { WM_NCMOUSELEAVE,                   L"WM_NCMOUSELEAVE"                   },
        { WM_WTSSESSION_CHANGE,              L"WM_WTSSESSION_CHANGE"              },
        { WM_DPICHANGED,                     L"WM_DPICHANGED"                     },
        { WM_DPICHANGED_BEFOREPARENT,        L"WM_DPICHANGED_BEFOREPARENT"        },
        { WM_DPICHANGED_AFTERPARENT,         L"WM_DPICHANGED_AFTERPARENT"         },
        { WM_GETDPISCALEDSIZE,               L"WM_GETDPISCALEDSIZE"               },
        { WM_CUT,                            L"WM_CUT"                            },
        { WM_COPY,                           L"WM_COPY"                           },
        { WM_PASTE,                          L"WM_PASTE"                          },
        { WM_CLEAR,                          L"WM_CLEAR"                          },
        { WM_UNDO,                           L"WM_UNDO"                           },
        { WM_RENDERFORMAT,                   L"WM_RENDERFORMAT"                   },
        { WM_RENDERALLFORMATS,               L"WM_RENDERALLFORMATS"               },
        { WM_DESTROYCLIPBOARD,               L"WM_DESTROYCLIPBOARD"               },
        { WM_DRAWCLIPBOARD,                  L"WM_DRAWCLIPBOARD"                  },
        { WM_PAINTCLIPBOARD,                 L"WM_PAINTCLIPBOARD"                 },
        { WM_VSCROLLCLIPBOARD,               L"WM_VSCROLLCLIPBOARD"               },
        { WM_SIZECLIPBOARD,                  L"WM_SIZECLIPBOARD"                  },
        { WM_ASKCBFORMATNAME,                L"WM_ASKCBFORMATNAME"                },
        { WM_CHANGECBCHAIN,                  L"WM_CHANGECBCHAIN"                  },
        { WM_HSCROLLCLIPBOARD,               L"WM_HSCROLLCLIPBOARD"               },
        { WM_QUERYNEWPALETTE,                L"WM_QUERYNEWPALETTE"                },
        { WM_PALETTEISCHANGING,              L"WM_PALETTEISCHANGING"              },
        { WM_PALETTECHANGED,                 L"WM_PALETTECHANGED"                 },
        { WM_HOTKEY,                         L"WM_HOTKEY"                         },
        { WM_PRINT,                          L"WM_PRINT"                          },
        { WM_PRINTCLIENT,                    L"WM_PRINTCLIENT"                    },
        { WM_APPCOMMAND,                     L"WM_APPCOMMAND"                     },
        { WM_THEMECHANGED,                   L"WM_THEMECHANGED"                   },
        { WM_CLIPBOARDUPDATE,                L"WM_CLIPBOARDUPDATE"                },
        { WM_DWMCOMPOSITIONCHANGED,          L"WM_DWMCOMPOSITIONCHANGED"          },
        { WM_DWMNCRENDERINGCHANGED,          L"WM_DWMNCRENDERINGCHANGED"          },
        { WM_DWMCOLORIZATIONCOLORCHANGED,    L"WM_DWMCOLORIZATIONCOLORCHANGED"    },
        { WM_DWMWINDOWMAXIMIZEDCHANGE,       L"WM_DWMWINDOWMAXIMIZEDCHANGE"       },
        { WM_DWMSENDICONICTHUMBNAIL,         L"WM_DWMSENDICONICTHUMBNAIL"         },
        { WM_DWMSENDICONICLIVEPREVIEWBITMAP, L"WM_DWMSENDICONICLIVEPREVIEWBITMAP" },
        { WM_GETTITLEBARINFOEX,              L"WM_GETTITLEBARINFOEX"              },

        { EM_GETSEL,                         L"EM_GETSEL"                         },
        { EM_SETSEL,                         L"EM_SETSEL"                         },
        { EM_GETRECT,                        L"EM_GETRECT"                        },
        { EM_SETRECT,                        L"EM_SETRECT"                        },
        { EM_SETRECTNP,                      L"EM_SETRECTNP"                      },
        { EM_SCROLL,                         L"EM_SCROLL"                         },
        { EM_LINESCROLL,                     L"EM_LINESCROLL"                     },
        { EM_SCROLLCARET,                    L"EM_SCROLLCARET"                    },
        { EM_GETMODIFY,                      L"EM_GETMODIFY"                      },
        { EM_SETMODIFY,                      L"EM_SETMODIFY"                      },
        { EM_GETLINECOUNT,                   L"EM_GETLINECOUNT"                   },
        { EM_LINEINDEX,                      L"EM_LINEINDEX"                      },
        { EM_SETHANDLE,                      L"EM_SETHANDLE"                      },
        { EM_GETHANDLE,                      L"EM_GETHANDLE"                      },
        { EM_GETTHUMB,                       L"EM_GETTHUMB"                       },
        { EM_LINELENGTH,                     L"EM_LINELENGTH"                     },
        { EM_REPLACESEL,                     L"EM_REPLACESEL"                     },
        { EM_GETLINE,                        L"EM_GETLINE"                        },
        { EM_SETLIMITTEXT,                   L"EM_SETLIMITTEXT"                   },
        { EM_CANUNDO,                        L"EM_CANUNDO"                        },
        { EM_UNDO,                           L"EM_UNDO"                           },
        { EM_FMTLINES,                       L"EM_FMTLINES"                       },
        { EM_LINEFROMCHAR,                   L"EM_LINEFROMCHAR"                   },
        { EM_SETTABSTOPS,                    L"EM_SETTABSTOPS"                    },
        { EM_SETPASSWORDCHAR,                L"EM_SETPASSWORDCHAR"                },
        { EM_EMPTYUNDOBUFFER,                L"EM_EMPTYUNDOBUFFER"                },
        { EM_GETFIRSTVISIBLELINE,            L"EM_GETFIRSTVISIBLELINE"            },
        { EM_SETREADONLY,                    L"EM_SETREADONLY"                    },
        { EM_SETWORDBREAKPROC,               L"EM_SETWORDBREAKPROC"               },
        { EM_GETWORDBREAKPROC,               L"EM_GETWORDBREAKPROC"               },
        { EM_GETPASSWORDCHAR,                L"EM_GETPASSWORDCHAR"                },
        { EM_SETMARGINS,                     L"EM_SETMARGINS"                     },
        { EM_GETMARGINS,                     L"EM_GETMARGINS"                     },
        { EM_GETLIMITTEXT,                   L"EM_GETLIMITTEXT"                   },
        { EM_POSFROMCHAR,                    L"EM_POSFROMCHAR"                    },
        { EM_CHARFROMPOS,                    L"EM_CHARFROMPOS"                    },
        { EM_SETIMESTATUS,                   L"EM_SETIMESTATUS"                   },
        { EM_GETIMESTATUS,                   L"EM_GETIMESTATUS"                   },
        { EM_ENABLEFEATURE,                  L"EM_ENABLEFEATURE"                  },

        { STM_SETICON,                       L"STM_SETICON"                       },
        { STM_GETICON,                       L"STM_GETICON"                       },
        { STM_SETIMAGE,                      L"STM_SETIMAGE"                      },
        { STM_GETIMAGE,                      L"STM_GETIMAGE"                      },

        { LB_ADDSTRING,                      L"LB_ADDSTRING"                      },
        { LB_INSERTSTRING,                   L"LB_INSERTSTRING"                   },
        { LB_DELETESTRING,                   L"LB_DELETESTRING"                   },
        { LB_SELITEMRANGEEX,                 L"LB_SELITEMRANGEEX"                 },
        { LB_RESETCONTENT,                   L"LB_RESETCONTENT"                   },
        { LB_SETSEL,                         L"LB_SETSEL"                         },
        { LB_SETCURSEL,                      L"LB_SETCURSEL"                      },
        { LB_GETSEL,                         L"LB_GETSEL"                         },
        { LB_GETCURSEL,                      L"LB_GETCURSEL"                      },
        { LB_GETTEXT,                        L"LB_GETTEXT"                        },
        { LB_GETTEXTLEN,                     L"LB_GETTEXTLEN"                     },
        { LB_GETCOUNT,                       L"LB_GETCOUNT"                       },
        { LB_SELECTSTRING,                   L"LB_SELECTSTRING"                   },
        { LB_DIR,                            L"LB_DIR"                            },
        { LB_GETTOPINDEX,                    L"LB_GETTOPINDEX"                    },
        { LB_FINDSTRING,                     L"LB_FINDSTRING"                     },
        { LB_GETSELCOUNT,                    L"LB_GETSELCOUNT"                    },
        { LB_GETSELITEMS,                    L"LB_GETSELITEMS"                    },
        { LB_SETTABSTOPS,                    L"LB_SETTABSTOPS"                    },
        { LB_GETHORIZONTALEXTENT,            L"LB_GETHORIZONTALEXTENT"            },
        { LB_SETHORIZONTALEXTENT,            L"LB_SETHORIZONTALEXTENT"            },
        { LB_SETCOLUMNWIDTH,                 L"LB_SETCOLUMNWIDTH"                 },
        { LB_ADDFILE,                        L"LB_ADDFILE"                        },
        { LB_SETTOPINDEX,                    L"LB_SETTOPINDEX"                    },
        { LB_GETITEMRECT,                    L"LB_GETITEMRECT"                    },
        { LB_GETITEMDATA,                    L"LB_GETITEMDATA"                    },
        { LB_SETITEMDATA,                    L"LB_SETITEMDATA"                    },
        { LB_SELITEMRANGE,                   L"LB_SELITEMRANGE"                   },
        { LB_SETANCHORINDEX,                 L"LB_SETANCHORINDEX"                 },
        { LB_GETANCHORINDEX,                 L"LB_GETANCHORINDEX"                 },
        { LB_SETCARETINDEX,                  L"LB_SETCARETINDEX"                  },
        { LB_GETCARETINDEX,                  L"LB_GETCARETINDEX"                  },
        { LB_SETITEMHEIGHT,                  L"LB_SETITEMHEIGHT"                  },
        { LB_GETITEMHEIGHT,                  L"LB_GETITEMHEIGHT"                  },
        { LB_FINDSTRINGEXACT,                L"LB_FINDSTRINGEXACT"                },
        { LB_SETLOCALE,                      L"LB_SETLOCALE"                      },
        { LB_GETLOCALE,                      L"LB_GETLOCALE"                      },
        { LB_SETCOUNT,                       L"LB_SETCOUNT"                       },
        { LB_INITSTORAGE,                    L"LB_INITSTORAGE"                    },
        { LB_ITEMFROMPOINT,                  L"LB_ITEMFROMPOINT"                  },
        { LB_GETLISTBOXINFO,                 L"LB_GETLISTBOXINFO"                 },

        { CB_GETEDITSEL,                     L"CB_GETEDITSEL"                     },
        { CB_LIMITTEXT,                      L"CB_LIMITTEXT"                      },
        { CB_SETEDITSEL,                     L"CB_SETEDITSEL"                     },
        { CB_ADDSTRING,                      L"CB_ADDSTRING"                      },
        { CB_DELETESTRING,                   L"CB_DELETESTRING"                   },
        { CB_DIR,                            L"CB_DIR"                            },
        { CB_GETCOUNT,                       L"CB_GETCOUNT"                       },
        { CB_GETCURSEL,                      L"CB_GETCURSEL"                      },
        { CB_GETLBTEXT,                      L"CB_GETLBTEXT"                      },
        { CB_GETLBTEXTLEN,                   L"CB_GETLBTEXTLEN"                   },
        { CB_INSERTSTRING,                   L"CB_INSERTSTRING"                   },
        { CB_RESETCONTENT,                   L"CB_RESETCONTENT"                   },
        { CB_FINDSTRING,                     L"CB_FINDSTRING"                     },
        { CB_SELECTSTRING,                   L"CB_SELECTSTRING"                   },
        { CB_SETCURSEL,                      L"CB_SETCURSEL"                      },
        { CB_SHOWDROPDOWN,                   L"CB_SHOWDROPDOWN"                   },
        { CB_GETITEMDATA,                    L"CB_GETITEMDATA"                    },
        { CB_SETITEMDATA,                    L"CB_SETITEMDATA"                    },
        { CB_GETDROPPEDCONTROLRECT,          L"CB_GETDROPPEDCONTROLRECT"          },
        { CB_SETITEMHEIGHT,                  L"CB_SETITEMHEIGHT"                  },
        { CB_GETITEMHEIGHT,                  L"CB_GETITEMHEIGHT"                  },
        { CB_SETEXTENDEDUI,                  L"CB_SETEXTENDEDUI"                  },
        { CB_GETEXTENDEDUI,                  L"CB_GETEXTENDEDUI"                  },
        { CB_GETDROPPEDSTATE,                L"CB_GETDROPPEDSTATE"                },
        { CB_FINDSTRINGEXACT,                L"CB_FINDSTRINGEXACT"                },
        { CB_SETLOCALE,                      L"CB_SETLOCALE"                      },
        { CB_GETLOCALE,                      L"CB_GETLOCALE"                      },
        { CB_GETTOPINDEX,                    L"CB_GETTOPINDEX"                    },
        { CB_SETTOPINDEX,                    L"CB_SETTOPINDEX"                    },
        { CB_GETHORIZONTALEXTENT,            L"CB_GETHORIZONTALEXTENT"            },
        { CB_SETHORIZONTALEXTENT,            L"CB_SETHORIZONTALEXTENT"            },
        { CB_GETDROPPEDWIDTH,                L"CB_GETDROPPEDWIDTH"                },
        { CB_SETDROPPEDWIDTH,                L"CB_SETDROPPEDWIDTH"                },
        { CB_INITSTORAGE,                    L"CB_INITSTORAGE"                    },
        { CB_GETCOMBOBOXINFO,                L"CB_GETCOMBOBOXINFO"                },

        { SBM_SETPOS,                        L"SBM_SETPOS"                        },
        { SBM_GETPOS,                        L"SBM_GETPOS"                        },
        { SBM_SETRANGE,                      L"SBM_SETRANGE"                      },
        { SBM_SETRANGEREDRAW,                L"SBM_SETRANGEREDRAW"                },
        { SBM_GETRANGE,                      L"SBM_GETRANGE"                      },
        { SBM_ENABLE_ARROWS,                 L"SBM_ENABLE_ARROWS"                 },
        { SBM_SETSCROLLINFO,                 L"SBM_SETSCROLLINFO"                 },
        { SBM_GETSCROLLINFO,                 L"SBM_GETSCROLLINFO"                 },
        { SBM_GETSCROLLBARINFO,              L"SBM_GETSCROLLBARINFO"              },
    };

}

//
// Public function definitions
//

CString GetExecutablePath( ) {
    wchar_t* pwsz;
    return { _get_wpgmptr( &pwsz ) ? L"" : pwsz };
}

CString GetWindowsMessageName( UINT const uMessage ) {
    CString result;

    if ( WindowsMessageNames.contains( uMessage ) ) {
        result = WindowsMessageNames.at( uMessage );
    } else if ( uMessage < WM_USER ) {
        result.Format( L"(0x%04X)", uMessage );
    } else if ( uMessage < WM_APP ) {
        result.Format( L"(WM_USER+0x%04X)", uMessage - WM_USER );
    } else {
        result.Format( L"(WM_APP+0x%04X)", uMessage - WM_APP );
    }

    return result;
}

void AdjustPosition( CWnd* pWnd, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition.left   += adjust.cx;
    wp.rcNormalPosition.top    += adjust.cy;
    wp.rcNormalPosition.right  += adjust.cx;
    wp.rcNormalPosition.bottom += adjust.cy;
    pWnd->SetWindowPlacement( &wp );
}

void AdjustPositionAndSize( CWnd* pWnd, SIZE const& adjustPosition, SIZE const& adjustSize ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition.left   += adjustPosition.cx;
    wp.rcNormalPosition.top    += adjustPosition.cy;
    wp.rcNormalPosition.right  += adjustPosition.cx + adjustSize.cx;
    wp.rcNormalPosition.bottom += adjustPosition.cy + adjustSize.cy;
    pWnd->SetWindowPlacement( &wp );
}

void AdjustSize( CWnd* pWnd, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  += adjust.cx;
    wp.rcNormalPosition.bottom += adjust.cy;
    pWnd->SetWindowPlacement( &wp );
}

void SetPosition( CWnd* pWnd, POINT const& position ) {
    SetPosition( pWnd, SIZE { position.x, position.y } );
}

void SetPosition( CWnd* pWnd, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  = wp.rcNormalPosition.right  - wp.rcNormalPosition.left + adjust.cx;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top  + adjust.cy;
    wp.rcNormalPosition.left   = adjust.cx;
    wp.rcNormalPosition.top    = adjust.cy;
    pWnd->SetWindowPlacement( &wp );
}

void SetPositionAndSize( CWnd* pWnd, RECT const& rect ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition = rect;
    pWnd->SetWindowPlacement( &wp );
}

void SetSize( CWnd* pWnd, SIZE const& size ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    pWnd->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  = wp.rcNormalPosition.left + size.cx;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top  + size.cy;
    pWnd->SetWindowPlacement( &wp );
}

[[nodiscard]] bool IsTextSelected( CEdit const* pEdit ) {
    int nStartIndex, nEndIndex;

    pEdit->GetSel( nStartIndex, nEndIndex );
    return nStartIndex != nEndIndex;
}

[[nodiscard]] CString GetWindowText( CEdit const* pEdit ) {
    CString str;
    if ( pEdit ) {
        pEdit->GetWindowText( str );
    }
    return str;
}

[[nodiscard]] bool GetValueFromEdit( CEdit const& edit, int& nValue ) {
    CString strText { GetWindowText( &edit ).Trim( ) };
    if ( strText.IsEmpty( ) ) {
        return false;
    }

    wchar_t* pwszEnd { };
    long tmp { wcstol( strText, &pwszEnd, 10 ) };
    if ( !pwszEnd || *pwszEnd ) {
        debug( L"GetValueFromEdit: garbage in number: '%s'\n", pwszEnd );
        return false;
    }
    if constexpr ( sizeof( long ) > sizeof( int ) ) {
        if ( ( tmp < static_cast<long>( INT_MIN ) ) || ( tmp > static_cast<long>( INT_MAX ) ) ) {
            debug( L"GetValueFromEdit: number out of range: %ld\n", tmp );
            return false;
        }
    }

    nValue = static_cast<int>( tmp );
    return true;
}

[[nodiscard]] bool GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged ) {
    int nOldValue { nValue };

    if ( GetValueFromEdit( edit, nValue ) ) {
        fChanged = nOldValue != nValue;
        return true;
    } else {
        return false;
    }
}

void PutValueToEdit( CEdit& edit, int const nValue ) {
    edit.SetWindowText( _FormatString( L"%d", nValue ) );
}

[[nodiscard]] DWORD SetWindowProcedure( HWND const hWnd, WNDPROC const newWndProc, WNDPROC& oldWndProc ) {
    SetLastError( ERROR_SUCCESS );
    oldWndProc = reinterpret_cast<WNDPROC>( SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( newWndProc ) ) );
    return ::GetLastError( );
}

[[nodiscard]] int ClipToChannelRange( AllChannels const channel, int const value ) {
    ChannelInformation const& channelInfo { AllChannelsInformation[+channel] };
    return std::min( std::max( value, channelInfo.m_minimumValue ), channelInfo.m_maximumValue );
}

void ComplainAboutBadValue( HWND hwnd, CEdit* pEdit, int const nNewValue, CString const& strMessage ) {
    PutValueToEdit( *pEdit, nNewValue );
    MessageBox( hwnd, strMessage, _GetResourceString( IDS_ERROR_CAPTION ), MB_OK | MB_ICONERROR );
    pEdit->SetFocus( );
}

[[nodiscard]] int BoolToChecked( bool const fValue ) {
    return fValue ? BST_CHECKED : BST_UNCHECKED;
}

void PutTextOnClipboard( CString const& str ) {
    SetLastError( ERROR_SUCCESS );

    if ( !::OpenClipboard( *AfxGetMainWnd( ) ) ) {
        DWORD dwError { ::GetLastError( ) };
        debug( L"PutTextOnClipboard: OpenClipboard failed: %lu\n", dwError );

        return;
    }

    size_t cbStr { ( static_cast<size_t>( str.GetLength( ) ) + 1 ) * sizeof( wchar_t ) };
    HGLOBAL hMem { ::GlobalAlloc( GMEM_DDESHARE, cbStr ) };
    if ( !hMem ) {
        DWORD dwError { ::GetLastError( ) };
        debug( L"PutTextOnClipboard: GlobalAlloc failed: %lu\n", dwError );

        ::CloseClipboard( );
        return;
    }

    wchar_t* pBuf { static_cast<wchar_t*>( ::GlobalLock( hMem ) ) };
    if ( !pBuf ) {
        DWORD dwError { ::GetLastError( ) };
        debug( L"PutTextOnClipboard: GlobalLock failed: %lu\n", dwError );

        ::GlobalFree( hMem );
        ::CloseClipboard( );
        return;
    }

    memcpy( pBuf, static_cast<LPCWSTR>( str ), cbStr );
    ::GlobalUnlock( hMem );

    HANDLE hClipboardData { ::SetClipboardData( CF_UNICODETEXT, hMem ) };
    if ( !hClipboardData ) {
        DWORD dwError { ::GetLastError( ) };
        debug( L"PutTextOnClipboard: SetClipboardData failed: %lu\n", dwError );

        ::GlobalFree( hMem );
        ::CloseClipboard( );
        return;
    }

    ::CloseClipboard( );
}

[[nodiscard]] CRect GetClientRect( CWnd* const pWnd ) {
    return GetClientRect( *pWnd );
}

[[nodiscard]] CRect GetClientRect( HWND const hwnd ) {
    CRect rect;
    return ::GetClientRect( hwnd, rect ) ? rect : BadRect;
}

[[nodiscard]] CRect GetScreenRect( CWnd* const pWnd ) {
    return GetScreenRect( *pWnd );
}

[[nodiscard]] CRect GetScreenRect( HWND const hwnd ) {
    CRect rect;
    if ( ::GetWindowRect( hwnd, rect ) ) {
        POINT lt { rect.TopLeft( ) };
        POINT rb { rect.BottomRight( ) };
        if ( ::ScreenToClient( hwnd, &lt ) && ::ScreenToClient( hwnd, &rb ) ) {
            return { lt, rb };
        }
    }

    return BadRect;
}

[[nodiscard]] CRect GetScreenRect( CWnd* const pParent, CWnd* const pWnd ) {
    return GetScreenRect( *pParent, *pWnd );
}

[[nodiscard]] CRect GetScreenRect( CWnd* const pParent, HWND const hwnd ) {
    return GetScreenRect( *pParent, hwnd );
}

[[nodiscard]] CRect GetScreenRect( HWND const hwndParent, CWnd* const pWnd ) {
    return GetScreenRect( hwndParent, *pWnd );
}

[[nodiscard]] CRect GetScreenRect( HWND const hwndParent, HWND const hwnd ) {
    CRect rect;
    if ( ::GetWindowRect( hwnd, rect ) ) {
        POINT lt { rect.TopLeft( ) };
        POINT rb { rect.BottomRight( ) };
        if ( ::ScreenToClient( hwndParent, &lt ) && ::ScreenToClient( hwndParent, &rb ) ) {
            return { lt, rb };
        }
    }

    return BadRect;
}

[[nodiscard]] CRect GetWindowRect( CWnd* const pWnd ) {
    return GetWindowRect( *pWnd );
}

[[nodiscard]] CRect GetWindowRect( HWND const hwnd ) {
    CRect rect;
    return ::GetWindowRect( hwnd, rect ) ? rect : BadRect;
}
