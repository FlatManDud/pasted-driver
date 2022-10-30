#include <ntddk.h>
#include <wdf.h>

#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x812, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)


#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x813, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

DRIVER_INITIALIZE DriverEntry;
UNICODE_STRING DriverName, SymbolName;

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	IoDeleteSymbolicLink(&SymbolName);
	IoDeleteDevice(pDriverObject->DeviceObject);
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS Status = STATUS_SUCCESS;
	return Status;
}

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath)
{

	PDEVICE_OBJECT pDeviceObj;
	RtlInitUnicodeString(&DriverName, L"\\Device\\CustomDriver");
	RtlInitUnicodeString(&SymbolName, L"\\DosDevices\\CustomDriver"); 
	UNICODE_STRING deviceNameUnicodeString, deviceSymLinkUnicodeString;
	NTSTATUS NtRet2 = IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObj);
	DriverObject->Flags |= DO_DIRECT_IO;
	DriverObject->Flags &= ~DO_DEVICE_INITIALIZING;
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	return STATUS_SUCCESS;
}

