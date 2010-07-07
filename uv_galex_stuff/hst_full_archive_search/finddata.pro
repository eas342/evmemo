;*********************************************************
; test_webget.pro
; retrieves specified url using webget function from
; Astron library
; (dum is a dummy parameter)
;
;*********************************************************
pro test_webget,dum

;
; search for HST data at given RA, Dec & radius,
; max_records=10, output is comma separated variables, 
; & only return requested columns.
;
url = "http://archive.stsci.edu/hst/search.php?"
url = url + "RA=53.084&DEC=-27.873&radius=1.0&max_records=10&";
url = url + "outputformat=CSV&action=Search"; 
url = url + "&selectedColumnsCsv=sci_data_set_name,sci_ra,sci_dec,sci_targname";

print,' '
print,"url = "+url
print,' '

;
; webget retruends an IDL structure
; with data in a.TEXT
; (assumes Astron library is in IDL search path)
;
a = webget(url)
; print,a.HEADER
print,a.Text
;
return
end
