#include <gtk/gtk.h>

GtkCssProvider* set_styles()
{
    GtkCssProvider* css;
    css = gtk_css_provider_new();
    // clang-format off
    gtk_css_provider_load_from_data(
            css,
            "/* Главное меню */"
            "window {"
                "color: #000;"
                "font-family: \"NanumSquareRound\";"
                "font-weight: bold;"
                "background-color: rgba(201, 221, 245, 1);"
            "}"
            
            "#btn_main {"
               " margin-right: 15px;"
                "margin-left: 15px;"
                "margin-bottom: 15px;"
            "}"
            
            "button {"
                "font-size:1.4rem;"
                "background-color:rgba(255, 255, 255, 0.8);"
                "border-radius: 50px;"
                "border-width: 3px;"
                "border-style: solid;"
                "border-color: rgb(160, 168, 255);"
                "color: rgb(160, 168, 255);"
                "outline-color: #007bff;"
                "outline-width: 3px;"
                "outline-style: solid;"
                "-gtk-outline-radius: 50px;"
            "}"
            
            "button:hover {"
                "color: #fff;"
                "background-color: #84B4FF;"
                "border-color: #007bff;"
                "box-shadow: 0 5px 15px rgba(0, 0, 0, 0.25), 0 4px 10px rgba(0, 123, 255, 0.25);"
                "}"
            
            "button:active {"
                "background-color: #84B7FF;"
                "border: 3px solid #A6A5FF;"
                "}"
            
            "button:disabled {"
                "background-color: rgba(127, 140, 141, 0.3);"
                "color: rgba(255,255,255,0.7);"
           "}"
            
            "#btn_main button {"
                "font-size:24px;"
            "}"
            "/* Меню словаря и статистики */"
            "treeview {"
                "background-color: RGB(255,255,255);"
                "font-size:14px;"
                "color: #007bff;"
                "border-width: 0px;"
                "border-right-width: 1px;"
                "border-left-width: 1px;"
                "border-bottom-width: 1px;"
                "border-style: solid;"
                "border-color: #007bff;"
            "}"
            "treeview:selected {"
                "background-color: rgba(124, 183, 255,0.2);"
            "}"
            "treeview:hover {"
                "background-color: rgba(255, 232, 232, 0.5);"
            "}"

            "treeview entry {"
                "padding: 0;"
                "font-size: 1.25rem;"
                "color: #495057;"
                "background-color: #fff;"
                "background-blend-mode: color;"
                "border: 1px solid #becad6;"
                "box-shadow: none;"
                "margin: 0;"
            "}"
            
            "treeview header button {"
                "background-blend-mode:color;"
                "border-width:0px;"
                "padding-top:0.5rem;"
                "padding-bottom:0.5rem;"
                "font-size:1rem;"
                "border-radius:1px;"
                "border-style: solid;"
                "color: #fff;"
                "background-color: #007bff;"
                "border-color: #007bff;"
                "box-shadow: 0 5px 15px rgba(0, 0, 0, 0.05), 0 4px 10px rgba(0, 123, 255, 0.25);"
            "}"

            "treeview header button:active {"
                "background-color: #84B7FF;"
                "border-width:0px;"
                
            "}"
            
            "treeview header button:first-child {"
                "border-top-left-radius: 15px;"
            "}"
            
            "treeview header button:last-child {"
                "border-top-right-radius: 15px;"
            "}"
            
            "#btn_voc {"
               "margin: 15px;"
            "}"
            
            "label#header {"
                "font-size:40px;"
                "color: #007bff;"
            "}"

            "spinner{"
                "color: #007bff;"
            "}"
            "/* Меню тренировки */"
            "#scrolled_win_training {"
                "color: #212529;"
                "border: 5px dotted rgba(90, 97, 105, 0.5);"
            "}"

            "label#label_name_window_results {"
                "border-bottom: 5px dotted rgba(90,97,105,0.5);"
            "}"
            "#btn_train {"
               "margin: 15px;"
            "}"
            "#btn_train button{"
               "font-size:1.4rem;"
            "}"

            "#button_success:disabled {"
                "background-color: rgba(23, 198, 113, 1);"
                "color: rgba(255,255,255,1);"
                "border-color: rgba(23, 198, 113, 1);"
            "}"

            "#button_failed:disabled {"
                "background-color: rgba(196, 24, 60, 1);"
                "color: rgba(255,255,255,1);"
                "border-color: rgba(196, 24, 60, 1);"
            "}"

            "#button_box_train {"
                "padding-top: 10px;"
                "border-top: 5px dotted rgba(90, 97, 105, 0.5);"
            "}"
            "#tr_box {"
                "margin-left: 5px;"
                "margin-right: 5px;"
                "margin-bottom: 10px;"

            "}"
            "#entry_answer_box {"
                "margin-left: 10rem;"
                "margin-right: 10rem;"
            "}"
            
            "entry {"
                "padding: 0.5rem 1rem;"
                "font-size: 2rem;"
                "color: #495057;"
                "background-color: #fff;"
                "border: 1px solid #becad6;"
                "font-weight: 300;"
                "border-radius: 0.375rem;"
                "box-shadow: none;"
                "transition: box-shadow 250ms cubic-bezier(0.27, 0.01, 0.38, 1.06), border 250ms cubic-bezier(0.27, 0.01, 0.38, 1.06);"
            "}"

           "entry:hover {"
                "border-color: #8fa4b8;"
            "}"

            "entry:disabled{"
                "background-color: #f5f6f7;"
            "}"

            "entry#entry_success:disabled {"
                "border-color: rgba(23, 198, 113, 1);"
            "}"

            "entry#entry_failed:disabled {"
                "border-color: rgba(196, 24, 60, 1);"
            "}"
            "",
            -1,
            NULL);
    // clang-format on
    return css;
}