.intel_syntax noprefix

.global AesKeyCaptureTrampoline
.global g_AesHookStolenBytesSize
.global g_AesCaptureTrampolineSize

.extern g_AesCapturedKey
.extern g_AesHookResumeAddress

.section .rdata
g_AesHookStolenBytesSize: .long AesHookStolenBytesEnd - AesHookStolenBytesBegin
g_AesCaptureTrampolineSize: .long AesKeyCaptureTrampolineEnd - AesKeyCaptureTrampoline

AesKeyCaptureTrampoline: # 15 bytes
    movabs r11, offset AesKeyCaptureHookStub
    jmp r11
    nop
    nop
AesKeyCaptureTrampolineEnd:

.text
AesKeyCaptureHookStub:
    movdqu xmm0, [rdx]
    movdqu [rip+g_AesCapturedKey], xmm0

AesHookStolenBytesBegin: # 15 bytes
    pushfq
    push r13
    mov r13, r13
    push 0x6FFF5A82
    mov [rsp], r14

AesHookStolenBytesEnd:
    jmp qword ptr [rip + g_AesHookResumeAddress]
