#pragma once

#include <fstream>
#include <iostream>
#include <cxxtest/TestSuite.h>

#include <xlnt/xlnt.hpp>
#include "helpers/path_helper.hpp"

class test_read : public CxxTest::TestSuite
{
public:
    void test_read_standalone_worksheet()
    {
        auto path = PathHelper::GetDataDirectory() + "/reader/sheet2.xml";
        xlnt::workbook wb;
        xlnt::worksheet ws(wb);
        {
            std::ifstream handle(path);
            ws = xlnt::reader::read_worksheet(handle, wb, "Sheet 2", {"hello"});
        }
        TS_ASSERT_DIFFERS(ws, nullptr);
        if(!(ws == nullptr))
        {
            TS_ASSERT_EQUALS(ws.get_cell("G5"), "hello");
            TS_ASSERT_EQUALS(ws.get_cell("D30"), 30);
            TS_ASSERT_EQUALS(ws.get_cell("K9"), 0.09);
        }
    }
    
    xlnt::workbook standard_workbook()
    {
        auto path = PathHelper::GetDataDirectory("genuine/emtpy.xlsx");
        return xlnt::load_workbook(path);
    }

    void test_read_standard_workbook()
    {
        TS_ASSERT_DIFFERS(standard_workbook(), nullptr);
    }

    void test_read_standard_workbook_from_fileobj()
    {
        auto path = PathHelper::GetDataDirectory("genuine/emtpy.xlsx");
        std::ifstream fo(path, std::ios::binary);
        auto wb = xlnt::load_workbook(path);
        TS_ASSERT_DIFFERS(standard_workbook(), nullptr);
    }

    void test_read_worksheet()
    {
        auto wb = standard_workbook()
        auto sheet2 = wb.get_sheet_by_name("Sheet2 - Numbers");
        TS_ASSERT_DIFFERS(sheet2, nullptr);
        TS_ASSERT_EQUALS("This is cell G5", sheet2.get_cell("G5"));
        TS_ASSERT_EQUALS(18, sheet2.get_cell("D18"));
        TS_ASSERT_EQUALS(true, sheet2.get_cell("G9"));
        TS_ASSERT_EQUALS(false, sheet2.get_cell("G10"));
    }

    void test_read_nostring_workbook()
    {
        auto path = PathHelper::GetDataDirectory("/genuine/empty-no-string.xlsx");
        auto wb = xlnt::load_workbook(path);
        TS_ASSERT_DIFFERS(standard_workbook(), nullptr);
    }

    void test_read_empty_file()
    {
        auto path = PathHelper::GetDataDirectory("/reader/null_file.xlsx";
        TS_ASSERT_THROWS(xlnt::load_workbook(path), xlnt::invalid_file_exception);
    }

    void test_read_empty_archive()
    {
        auto path = PathHelper::GetDataDirectory("/reader/null_archive.xlsx");
        TS_ASSERT_THROWS(xlnt::load_workbook(path), xlnt::invalid_file_exception);
    }
    
    void test_read_workbook_with_no_properties()
    {
        auto path = PathHelper::GetDataDirectory("/reader/null_archive.xlsx");
        xlnt::load_workbook(path);
    }

    void workbook_with_styles()
    {
        auto path = PathHelper::GetDataDirectory("/reader/empty-with-styles.xlsx");
        return xlnt::load_workbook(path);
    }

    void test_read_workbook_with_styles_general()
    {
        auto wb = workbook_with_styles();
        auto ws = wb["Sheet1"];
        TS_ASSERT(ws.get_cell("A1").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::general);
    }
    
    void test_read_workbook_with_styles_date()
    {
        auto wb = workbook_with_styles();
        auto ws = wb["Sheet1"];
        TS_ASSERT(ws.get_cell("A2").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::date_xlsx14);
    }
    
    void test_read_workbook_with_styles_number()
    {
        auto wb = workbook_with_styles();
        auto ws = wb["Sheet1"];
        TS_ASSERT(ws.get_cell("A3").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::number00);
    }
    
    void test_read_workbook_with_styles_time()
    {
        auto wb = workbook_with_styles();
        auto ws = wb["Sheet1"];
        TS_ASSERT(ws.get_cell("A4").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::date_time_3);
    }
    
    void test_read_workbook_with_styles_percentage()
    {
        auto wb = workbook_with_styles();
        auto ws = wb["Sheet1"];
        TS_ASSERT(ws.get_cell("A5").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::percentage00);
    }
    
    void date_mac_1904()
    {
        auto path = PathHelper::GetDataDirectory("/reader/date_1904.xlsx");
        return xlnt::load_workbook(path);
    }
    
    void date_std_1900()
    {
        auto path = PathHelper::GetDataDirectory("/reader/date_1900.xlsx");
        return xlnt::load_workbook(path);
    }
    
    void test_read_win_base_date()
    {
        auto wb = date_std_1900();
        TS_ASSERT_EQUALS(wb.get_properties().get_excel_base_date(), xlnt::calendar::windows_1900);
    }
    
    void test_read_mac_base_date()
    {
        auto wb = date_mac_1904();
        TS_ASSERT_EQUALS(wb.get_properties().get_excel_base_date(), xlnt::calendar::mac_1904);
    }

    void test_read_date_style_win()
    {
        auto wb = date_std_1900();
        auto ws = wb["Sheet1"];
        TS_ASSERT_EQUALS(ws.get_cell("A1").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::date_xlsx14);
    }

    void test_read_date_style_mac()
    {
        auto wb = date_mac_1904();
        auto ws = wb["Sheet1"];
        TS_ASSERT_EQUALS(ws.get_cell("A1").get_style().get_number_format().get_format_code() == xlnt::number_format::known_formats::date_xlsx14);
    }

    void test_read_win_base_date()
    {
        auto wb_mac = date_mac_1904();
        auto ws_mac = wb_mac["Sheet1"];
        auto wb_win = date_win_1900();
        auto ws_win = wb_win["Sheet1"];
        xlnt::datetime dt(2011, 10, 31);
        TS_ASSERT_EQUALS(ws_mac.get_cell("A1"), dt);
        TS_ASSERT_EQUALS(ws_win.get_cell("A1"), dt);
        TS_ASSERT_EQUALS(ws_mac.get_cell("A1"), ws_win.get_cell("A1"));
    }
    
    void test_repair_central_directory()
    {
        TS_ASSERT(false);
        /*
        std::string data_a = "foobarbaz" + xlnt::CentralDirectorySignature;
        std::string data_b = "bazbarfoo12345678901234567890";
        
        auto f = xlnt::repair_central_directory(data_a + data_b, true);
        TS_ASSERT_EQUALS(f, data_a + data_b.substr(0, 18));
        
        f = xlnt::repair_central_directory(data_b, true);
        TS_ASSERT_EQUALS(f, data_b);
        */
    }
    
    void test_read_no_theme()
    {
        auto path = PathHelper::GetDataDirectory("/genuine/libreoffice_nrt.xlsx");
        auto wb = xlnt::load_workbook(path);
        TS_ASSERT_DIFFERS(wb, nullptr);
    }
    
    void test_read_cell_formulae()
    {
        xlnt::workbook wb;
        auto ws = wb.get_active_sheet();
        auto path = PathHelper::GetDataDirectory("/reader/worksheet_formula.xml");
        std::ifstream ws_stream(path);
        xlnt::fast_parse(ws, ws_stream, {"", ""}, {}, 0);
        
        auto b1 = ws.get_cell("B1");
        TS_ASSERT_EQUALS(b1.get_data_type(), xlnt::cell::type::formula);
        TS_ASSERT_EQUALS(b1, "=CONCATENATE(A1, A2)");
        
        auto a6 = ws.get_cell("A6");
        TS_ASSERT_EQUALS(a6.get_data_type(), xlnt::cell::type::formula);
        TS_ASSERT_EQUALS(a6, "=SUM(A4:A5)");
    }
    
    void test_read_complex_formulae()
    {
        TS_ASSERT(false);
    }
    
    void test_data_only()
    {
        TS_ASSERT(false);
    }
    
    void test_detect_worksheets()
    {
        TS_ASSERT(false);
    }
    
    void test_read_rels()
    {
        TS_ASSERT(false);
    }
    
    void test_read_content_types()
    {
        std::vector<std::pair<std::string, std::string>> expected = 
        {
            {"/xl/workbook.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"},
            {"/xl/worksheets/sheet1.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"},
            {"/xl/chartsheets/sheet1.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.chartsheet+xml"},
            {"/xl/worksheets/sheet2.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"},
            {"/xl/theme/theme1.xml", "application/vnd.openxmlformats-officedocument.theme+xml"},
            {"/xl/styles.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"},
            {"/xl/sharedStrings.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"},
            {"/xl/drawings/drawing1.xml", "application/vnd.openxmlformats-officedocument.drawing+xml"},
            {"/xl/charts/chart1.xml", "application/vnd.openxmlformats-officedocument.drawingml.chart+xml"},
            {"/xl/drawings/drawing2.xml", "application/vnd.openxmlformats-officedocument.drawing+xml"},
            {"/xl/charts/chart2.xml", "application/vnd.openxmlformats-officedocument.drawingml.chart+xml"},
            {"/xl/calcChain.xml", "application/vnd.openxmlformats-officedocument.spreadsheetml.calcChain+xml"},
            {"/docProps/core.xml", "application/vnd.openxmlformats-package.core-properties+xml"},
            {"/docProps/app.xml", "application/vnd.openxmlformats-officedocument.extended-properties+xml"}
        };
        
        auto path = PathHelper::GetDataDirectory("/reader/bug137.xlsx");
        xlnt::zip_file f(path);
        auto result = xlnt::workbook_reader::read_conent_types(f);
            
        TS_ASSERT_EQUALS(result, expected);
    }
    
    void test_read_sheets()
    {
        {
            auto path = PathHelper::GetDataDirectory("/reader/bug137.xlsx");
            xlnt::zip_file f(path);
            auto sheets = xlnt::workbook_reader::read_sheets(f);
            TS_ASSERT_EQUALS(sheets["rId1"], "Chart1");
            TS_ASSERT_EQUALS(sheets["rId2"], "Sheet1");
        }
        
        {
            auto path = PathHelper::GetDataDirectory("/reader/bug304.xlsx");
            xlnt::zip_file f(path);
            auto sheets = xlnt::workbook_reader::read_sheets(f);
            TS_ASSERT_EQUALS(sheets["rId1"], "Sheet1");
            TS_ASSERT_EQUALS(sheets["rId2"], "Sheet2");
            TS_ASSERT_EQUALS(sheets["rId3"], "Sheet3");
        }
    }
    
    void test_guess_types()
    {
        bool guess;
        xlnt::cell:type dtype;
        
        for(const auto &expected : {true, xlnt::cell::type::number}, {false, xlnt::cell::type::string})
        {
            std::tie(guess, dtype) = expected;
            auto path = PathHelper::GetDataDirectory("/genuine/guess_types.xlsx");
            auto wb = xlnt::load_workbook(path, guess);
            auto ws = wb.get_active_sheet();
            TS_ASSERT_EQUALS(ws.get_cell("D2").get_data_type(), dtype);
        }
    }
    
    void test_read_autofilter()
    {
        auto path = PathHelper::GetDataDirectory("/reader/bug275.xlsx");
        auto wb = xlnt::load_workbook(path);
        auto ws = wb.get_active_sheet();
        TS_ASSERT_EQUALS(ws.get_auto_filter.get_reference(), "A1:B6");
    }
    
    void test_bad_formats_xlsb()
    {
        auto path = PathHelper::GetDataDirectory("/genuine/a.xlsb");
        TS_ASSERT_THROWS(xlnt::load_workbook(path), xlnt::invaid_file_exception);
    }
    
    void test_bad_formats_xls()
    {
        auto path = PathHelper::GetDataDirectory("/genuine/a.xls");
        TS_ASSERT_THROWS(xlnt::load_workbook(path), xlnt::invaid_file_exception);
    }
    
    void test_bad_formats_no()
    {
        auto path = PathHelper::GetDataDirectory("/genuine/a.no-format");
        TS_ASSERT_THROWS(xlnt::load_workbook(path), xlnt::invaid_file_exception);
    }
};
